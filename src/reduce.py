import numpy as np
import tensorflow as tf
import struct
import math
import sys
import random
import torch
import matplotlib.pyplot as plt
from array import array
from tensorflow.keras import backend as K
from tensorflow import keras
from tensorflow.keras import Sequential
from tensorflow.keras.layers import Input,Conv2D,Flatten,Reshape,Conv2DTranspose,MaxPooling2D,UpSampling2D,BatchNormalization,Conv2D,Dropout,Dense
from tensorflow.keras.models import Model
from tensorflow.keras.optimizers import RMSprop
from sklearn.model_selection import train_test_split

def show_images(images, title_texts):
    cols = 5
    rows = int(len(images)/cols) + 1
    plt.figure(figsize=(30,20))
    index = 1    
    for x in zip(images, title_texts):        
        image = x[0]        
        title_text = x[1]
        plt.subplot(rows, cols, index)        
        plt.imshow(image, cmap=plt.cm.gray)
        if title_text != '':
            plt.title(title_text, fontsize = 15);        
        index += 1
    plt.savefig('images.jpg')
    # plt.show()

def Load_Mnist_Images(train_images_path):
    with open(train_images_path, 'rb') as file:
        magic_num, num_of_images, rows, cols = struct.unpack(">IIII", file.read(16))
        image_data = array("B", file.read())        
    images = []
    for i in range(num_of_images):
        images.append([0] * rows * cols)
    for i in range(num_of_images):
        img = np.array(image_data[i * rows * cols:(i + 1) * rows * cols])
        img = img.reshape(28, 28)
        images[i][:] = img            
    print("Number of images: ",num_of_images)
    print("Rows of images: ",rows)
    print("Cols of images: ",cols)
    return images

def Save_MNIST_Images(data, output_file):
    header = np.array([0x0803, len(data), 1, data.shape[1]], dtype='>i4')

    with open(output_file, "wb") as f:
        f.write(header.tobytes())

        for i in range(0,data.shape[0]):
            a=bytearray()
            for j in range(0,data.shape[1]):
                item = int(data[i][j])
                item = item.to_bytes(2,byteorder='big')
                a.extend(item)
            f.write(a)

class AutoEncoder():

  def __init__(self, input_image):
    self.input_image = input_image
  
  def Encoder(self):
    conv = self.input_image
    
    # Add Convolutional layer
    conv = Conv2D(32, (3, 3), activation='relu', strides=2, padding='same')(conv)
    # Add Convolutional layer
    conv = Conv2D(64, (3, 3), activation='relu', strides=2, padding='same')(conv)
    # Add Convolutional layer
    conv = Conv2D(128, (3, 3), activation='relu', strides=3, padding='same')(conv)

    # We need this shape later in the decoder, so we save it into a variable.
    encoded_shape = K.int_shape(conv)

    # Add a Flatten layer
    conv = Flatten()(conv)
    # Add a Dense layer to reduce the dimensions to 10
    encoded = Dense(10, name='encoder')(conv)
      
    return (encoded, encoded_shape)

  def Decoder(self,convolution_layer,encoded_shape):
    conv = convolution_layer

    # Add a Dense layer with help of encoded shape that we saved previously 
    conv = Dense(np.prod(encoded_shape[1:]))(conv)
    # Reshape this to be (3,3,128)
    conv = Reshape((encoded_shape[1], encoded_shape[2], encoded_shape[3]))(conv)
    # Add a Convolution 2D Transpose layer, with padding='valid, so as to upsampling from (3,3) to (7,7)
    conv = Conv2DTranspose(64, (3,3), activation='relu',strides=2, padding='valid')(conv)
    # Add a Convolution 2D Transpose layer
    conv = Conv2DTranspose(32,(3, 3), activation='relu', strides=2, padding='same')(conv)
    # Add a final Convolution 2D Transpose layer
    conv = Conv2DTranspose(1,(3, 3), activation='sigmoid', strides=2, padding='same')(conv)
    
    return conv  

  def call(self):
    (convolution_layer, encoded_shape) = self.Encoder()
    recostructed_layer = self.Decoder(convolution_layer,encoded_shape)
    return recostructed_layer

def main(argv):

    if(len(sys.argv) != 9):
        print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>>")
        exit(1)
    else:
        if(str(sys.argv[1]) != '-d'):
            print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>")
            exit(1)
        else:
            train_file = str(sys.argv[2])

        if(str(sys.argv[3]) != '-q'):
            print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>")
            exit(1)
        else:
            test_file = str(sys.argv[4])

        if(str(sys.argv[5]) != '-od'):
            print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>")
            exit(1)
        else:
            output_train_file = str(sys.argv[6])

        if(str(sys.argv[7]) != '-oq'):
            print("Sorry, the input must be in this form: reduce.py  –d  <dataset>  -q  <queryset>  -od  <output_dataset_file>  -oq  <output_query_file>")
            exit(1)
        else:
            output_test_file = str(sys.argv[8])
        

    train_data = Load_Mnist_Images(train_file)

    #Rescale the training data with the maximum pixel value of them
    train_data = train_data / np.max(train_data)
    train_data = train_data.reshape(train_data.shape[0], train_data.shape[1], train_data.shape[2])
    train_data = train_data.astype('float64')

    # print(train_data[0])
    print("Training set (images) shape: {shape}".format(shape=train_data.shape))

    train_data = train_data.reshape(-1, train_data.shape[1],train_data.shape[1], 1)

    print(train_data.shape)
    print(train_data.dtype)
    print(np.max(train_data))
    print(np.min(train_data))

    # Train test split
    X_train, X_val, ground_train, ground_val = train_test_split(train_data, train_data, test_size=0.2, random_state=13)

    epochs = 20
    batch_size = 128

    # Shape of each photo
    input_img = Input(shape = (28, 28, 1))
    # Define our instance of class AutoEncoder
    autoenc = AutoEncoder(input_img)
    # Define our model
    autoencoder = Model(input_img, autoenc.call())
    # Compile our model
    autoencoder.compile(loss='mean_squared_error', optimizer = RMSprop())  
    print(autoencoder.summary()) 
    # Train our model
    autoencoder_train = autoencoder.fit(X_train, ground_train, batch_size=batch_size, epochs=epochs, shuffle=True, verbose=1, validation_data=(X_train, ground_train))

    # Plot the train and validation losses
    N = np.arange(0, epochs)
    plt.figure(figsize=(10,8))
    plt.plot(N, autoencoder_train.history['loss'], label='train_loss')
    plt.plot(N, autoencoder_train.history['val_loss'], label='val_loss')
    plt.title('Training Loss and Accuracy')
    plt.xlabel('Epochs')
    plt.ylabel('Loss/Accuracy')
    plt.legend(loc='upper right')

    path = "images/A/" + "train_val_losses" + ".jpg" 
    plt.savefig(path)

    # Take the encoder 
    encoder = Model(inputs=autoencoder.input,outputs=autoencoder.get_layer('encoder').output)
    encoder.summary()

    test_data = Load_Mnist_Images(test_file)

    #Rescale the test data with the maximum pixel value of them
    test_data = test_data / np.max(test_data)
    test_data = test_data.reshape(test_data.shape[0], test_data.shape[1], test_data.shape[2])
    test_data = test_data.astype('float64')

    print("Test set (images) shape: {shape}".format(shape=test_data.shape))

    test_data = test_data.reshape(-1, test_data.shape[1],test_data.shape[1], 1)

    # Make the predictions to train and test data
    train = encoder.predict(train_data)
    test = encoder.predict(test_data)

    print("Train shape: ",train.shape)
    print("Test shape: ",test.shape)

    # Normalize the values to be in space [0, 25500]
    train = ((train-np.min(train))/(np.max(train)-np.min(train)))*25500
    test = ((test-np.min(test))/(np.max(test)-np.min(test)))*25500

    train = np.round(train,0)
    test = np.round(test,0)

    train = train.astype('uint16')
    test = test.astype('uint16')

    print("Maximun value of train: ",np.min(train))
    print("Minimun value of train: ",np.max(train))

    # Save the data in files like MNIST file
    Save_MNIST_Images(train, output_train_file)
    Save_MNIST_Images(test, output_test_file)


if __name__ == "__main__":
    main(sys.argv[0:])
import numpy as np
import tensorflow as tf
import struct
import math
import sys
import random
# import torch
import matplotlib.pyplot as plt
from array import array

# a = np.arange(4).reshape(1,4)
# a = np.array([1, 2, 3, 4])
# # a = a/255

# print(a)

# new = bytearray(a)

# new = bytes(new)

# print(new)
# # print(len(a))

# # s = struct.pack('<i', a)

# print(s)
# temp = 0

# number1 = (1 & 0b11110000) >> 4
# number2 = (2 & 0b00001111)

# x = (number1 << 4) | number2

# s = struct.pack('>i', 5) 
s=1
# b=2
s = s.to_bytes(2, byteorder = 'big')
# s = s.to_bytes(2, byteorder = 'big')

print("hiiii",type(s))

# s = int.from_bytes(s, "big")

# print(s)

array = np.array(([11699, 12289, 15367, 10128, 14961, 15238, 11439, 12821, 10368, 13379])).reshape(1,10)

print(array)
print(array.dtype)
print(array.shape)

new = np.zeros((1,10), dtype='bytes')
print(new)
print(new.shape)
print((new.dtype))

# def convert2bytes(arr):
#     return arr.to_bytes(2, byteorder='big')

# array = convert2bytes(array)

# array = np.where(array==array, array.to_bytes(2, byteorder='big'), array) 

# con = lambda x: x.to_bytes(2, byteorder='big')

# array = con(array)

# array = map(to_bytes(2, byteorder='big'),array)

# array = np.array(list(map(to_bytes(2, byteorder='big'), array)))
a=bytearray()
temp = 1

for i in np.nditer(array):
    i = int(i)
    # print(i)
    # print(type(i))
    # if(temp!=1):
    #     a.extend(i)
    i = i.to_bytes(2,byteorder='big')

    print(i,"\n")
    
    # print(i)
    # print(type(i))
    # new[a] = i
    # a += 1
    a.extend(i)
    temp=0


print(a)

# 11086

s = int.from_bytes(b',\xc0'  , "big")
print("aytoooooooooooooooooo  ", s)

# s = 14961

array = array.tobytes()

print("heyy", array)

s=11086
s = s.to_bytes(2, byteorder = 'little')
print("edvvvvvvvvvvvvvvvvvvvvvvv",s)

s = int.from_bytes(b',\xc0'  , "big")
print("aytoooooooooooooooooo     ", s)

# print(a[0])

# new += array

# for i in range(10):
#     print("ffff ",i)
#     print(new[i])
#     print(array[i])
#     new[i]+=array[i]
#     print(new[i])

# print(new)



# for i in a:
#     # new = a.tobytes()
#     print(i)
#     if temp == 0:
#         one = i
#         temp = 1
#     elif temp == 1:
#         two = i 
#         pixel = (one  << 8) | (two)
#         temp = 0


# print(bytes(pixel))

for i in range(0,28,7):
    print("hi: ", i)

s = '1'
s1 = "1"

print(isinstance(s, str))
print(isinstance(s1, str))
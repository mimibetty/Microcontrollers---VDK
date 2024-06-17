import numpy as np 

 

# f(x) 

def f(x): 
  return (np.exp(x) - 4 / np.exp(x)) ** 2 

 

def df(x): 
  return 2 * (np.exp(x) - 4 * np.exp(-x)) * (np.exp(x) + 4 * np.exp(-x)) 

 

def gradient_ascent(df, initial_x, learning_rate, n_iterations, tolerance): 
  x = initial_x 
  for _ in range(n_iterations): 
    print(_) 
    grad = df(x) 
    print(grad)
    if abs(grad) < tolerance: # Dừng nếu đạo hàm đủ nhỏ 
      break 
    x += learning_rate * grad # Cập nhật x theo hướng dốc đạo hàm 
    print(x)
  return x 

 

# Các tham số cho Gradient Ascent 

initial_x = 0.0  
learning_rate = 0.01 # Learning rate 
n_iterations = 1000 # Số lần lặp tối đa 
tolerance = 1e-6 # Ngưỡng dừng 
 

# Tìm điểm cực đại 
x_max = gradient_ascent(df, initial_x, learning_rate, n_iterations, tolerance) 
f_max = f(x_max) 
print(f"Điểm cực đại x: {x_max}") 
print(f"Giá trị lớn nhất của hàm số f(x): {f_max}") 

 

 x  = 3 
k = 2
0 , 1 ,2 ,3 
1 , 4, 7 , 10
x = 1 
for (int i = 0; i < group1,2,3,4,5.size(); i++) {
  int l= 0, r = i;
  while (l < r) {
      ps[]
      mid -> i 
      tong = ps[i-1] - ps[mid-1] // tong mid -> i-1  => muốn biến thành a[i] 
      tong + K* = a[i] * (i - mid - 1) 
      => K* > K => ko được l = mid + 1
      K* <= K  => r = mid 

  }
  l = 2;  // a=4  
  res = max(res, i - l + 1)
}


2 , 5 , 8 , 11

sorry i gave u some misunderstand information, the place which i recommend to you is "Phuoc Kien Assembly Hall", for vietnamese people call it "Hội Quán Phước Kiến".
this is some usefull information if you want to visit there or anywhere near them 
https://vinpearl.com/vi/cam-nang-kham-pha-hoi-quan-phuc-kien-hoi-an-tu-a-z-khong-nen-bo-lo
** you can use gg to translate this page 😉
in Hoi An Ancient Town, There are also many destinations related to history and culture, you can explore each location one by one if you want
Bạn đã gửi

Regarding specialties, I recommend you to enjoy Pho and Banh Beo, here is the address for each dish Banh Beo usually opens after 2pm https://maps.app.goo.gl/2h672oVsrxDW Why48
There are many pho restaurants but I really recommend Pho Muoi "Pho Muoi", it's only open in the morning. Maybe it's closed because of a holiday, I don't know but I'll give you the address https://maps.app.goo.gl/qz9UgkjT2jBie8Gw6
Bạn đã gửi
Hope they are all useful to you. Wish you have a memorable trip
Bạn đã gửi
sorry i gave u some misunderstand information, the place which i recommend to you is "Phuoc Kien Assembly Hall", for vietnamese people call it "Hội Quán Phước Kiến".
this is some usefull information if you want to visit there or anywhere near them 
https://vinpearl.com/vi/cam-nang-kham-pha-hoi-quan-phuc-kien-hoi-an-tu-a-z-khong-nen-bo-lo
** you can use gg to translate this page 😉
in Hoi An Ancient Town, There are also many destinations related to history and culture, you can explore each location one by one if you want
Bạn đã gửi
Hello Laure, I'm the person who met you last night at the noodle shop
sorry i gave u some misunderstand information, the place which i recommend to you is "Phuoc Kien Assembly Hall", for vietnamese people call it "Hội Quán Phước Kiến". You can search this keyword on the internet to have more information about this place and somewhere nearby
in Hoi An Ancient Town, There are also many destinations related to history and culture, you can explore each location one by one if you want
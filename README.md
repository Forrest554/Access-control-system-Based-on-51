# Access-control-system-Based-on-51
## 项目简介
该项目基于集成的51单片机（淘宝出售的那种），完成具有时间显示、密码输入、IC模块输入、红外输入的门禁系统。具体工功能如下：
1.	按下“时钟”键初始化时钟，并显示当前的年、月、日、周、时、分、秒，可以随时查看当前时间；
2.	按下“上一页”在LCD1602上显示密码输入和密码修改两个两个功能，按下“下一页”在LCD1602上显示刷卡输入；
3.	在密码输入时，输入密码伴有蜂鸣器，如果输入错误可以退格，密码正确并且确认，蜂鸣器发声，继电器打开；
4.	在修改密码时，输入原密码，新密码，同时保存到Eeprom中，使得断电后密码不变；
5.	在刷卡输入模块中，选择输入时，刷完卡完后显示卡号并打开继电器，选择增加卡时，增加新的卡用户；
6.	红外遥控按下开机键后，继电器打开；

--------
### 部分连线
#### RC552
```
// RC522 SPI接口定义  
sbit     MF522_NSS  =    P3^0;      //这是SDA  
sbit     MF522_SCK  =    P3^3;  
sbit     MF522_SI   =    P3^4;  
sbit     MF522_SO   =    P3^5;  
sbit     MF522_RST  =    P3^6;      //RC500片选  
```
####继电器
```
sbit relay = P3^1;   
```
####[其余连线](https://github.com/Forrest554/Access-control-system-Based-on-51/blob/master/image/HC6800-ES.pdf)

###效果展示
![blockchain](https://github.com/Forrest554/Access-control-system-Based-on-51/blob/master/image/demo.png?raw=true)

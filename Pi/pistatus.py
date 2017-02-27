from datetime import datetime
import os
#import netifaces as ni
#ni.ifaddresses('wlan0')
#ip = ni.ifaddresses('wlan0')[2][0]['addr']

f = os.popen('ifconfig wlan0 | grep "inet\ addr" | cut -d: -f2 | cut -d" " -f1')
your_ip=f.read()
file_path = "/home/pi/pistatus.txt"

time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
text_file = open(file_path, "w")
text_file.write("Czas logu: " + time + "\n")
text_file.write("Adres IP: " + your_ip)
#text_file.write ("Adres IP2 : " + ip)
text_file.close ()
cmd = "/home/pi/dropbox_uploader.sh upload " + file_path + "  pistatus.txt" 
os.system(cmd)

f.close() 


import os

os.system("sudo rm /home/pi/motion/tmp/" + "*.jpg")
os.system("sudo rm /home/pi/motion/tmp/webcam1/" + "*.jpg")
os.system("sudo rm /home/pi/motion/tmp/webcam2/" + "*.jpg")
os.system("sudo rm /var/www/html/webcam1/" + "*.jpg")
os.system("sudo rm /var/www/html/webcam2/" + "*.jpg")


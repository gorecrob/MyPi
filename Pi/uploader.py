import os

from datetime import datetime

pathw1="/var/www/html/webcam1/"
pathw2="/var/www/html/webcam2/"
path="/home/pi/motion/"
path_arch = "archive/"
dest="/MalinaBencol/"

year = datetime.now ().strftime ('%Y')
month = datetime.now ().strftime ('%m')
day = datetime.now ().strftime ('%d')
def upload_files():

    if not os.path.exists(pathw1):

        return

    dir_list1 = os.listdir(pathw1)

    for file_name1 in dir_list1:

        if file_name1.endswith(".avi"):

            cmd = "sudo /home/pi/dropbox_uploader.sh upload " + pathw1 + file_name1 + " "+ datetime.now().strftime('%Y-%m-%d') + "/webcam1/" + file_name1

            os.system(cmd)

            if not os.path.exists(path + path_arch):
                os.system ('mkdir ' + path + path_arch)

            if not os.path.exists (path + path_arch + "/" + year + "/"):
                os.system ("mkdir " + path + path_arch + "/" + year)

            os.system("sudo mv /var/www/html/webcam1/" + file_name1 + " /home/pi/motion/archive/" + file_name1)



    if not os.path.exists(pathw2):
        return

    dir_list2 = os.listdir(pathw2)
    for file_name2 in dir_list2:
        if file_name2.endswith(".avi"):
            cmd = "sudo /home/pi/dropbox_uploader.sh upload " + pathw2 + file_name2 + " "+ datetime.now().strftime('%Y-%m-%d') + "/webcam2/" + file_name2
            os.system(cmd)
            if not os.path.exists(path + path_arch):
                os.system ('mkdir ' + path + path_arch)
            if not os.path.exists (path + path_arch + "/" + year + "/"):
                os.system ("mkdir " + path + path_arch + "/" + year)
            os.system("sudo mv /var/www/html/webcam2/" + file_name2 + " /home/pi/motion/archive/" + file_name2)


#if _name_ == "_main_":

upload_files()


import urllib.request
import smtplib
from datetime import datetime

u = 'http://otomoto.pl/osobowe/renault/megane/iii//kombi,mpv,van-minibus?s=p&l=60&fq%5Blocation_type%5D=region&fq%5Bregion%5D%5B0%5D=ma%C5%82opolskie&fq%5Bregion%5D%5B1%5D=opolskie&fq%5Bregion%5D%5B2%5D=%C5%9Bl%C4%85skie&fq%5Bodometer%5D%5Bto%5D=100000&fq%5Btechnical_condition%5D=functioning&fq%5Bhistory%5D%5B0%5D=item.has_no_accident'
f = urllib.request.urlopen(u)
contents = str(f.read())
f.close()
i = 0
j = 0
counter = 1
ilosc_olgoszen = 0
ilosc_nowych = 0
kryteria = 'kryteria: <strong>'
ogloszenia = 'class="om-list-item"><h3><a href="/renault-megane-iii'
lista = []
listaNowych = []
listaStarych = []
ogloszeniaOld = ('/home/pi/ogloszeniaOld.txt')

def wczytajOgloszenia ():
    global u,i,j,contents,counter, ilosc_ogloszen, kryteria, ogloszenia, lista
    while True:
        i = contents.find(kryteria, i)
        if i == -1:
            print ('Nie znaleziono')
            break
#        print ('Znaleziono ogłoszeń:')
        end_ilosc = contents.find('<',i+18)
        ilosc_ogloszen = int(contents[i+18:end_ilosc])   
#        print (ilosc_ogloszen)
        break
    while True:
        j = contents.find (ogloszenia, j)
        if j == -1:
            break
        koniec_linku = contents.find('>',j + len(ogloszenia))
        lista.insert(counter,  ('http://otomoto.pl/renault-megane-iii' + contents[j+len(ogloszenia):koniec_linku - 1]+ '\n'))
        j = koniec_linku 
        counter = counter + 1


def wczytajOgloszeniaOld ():
    global ogloszeniaOld
    plikOgloszeniaOld = open (ogloszeniaOld)
    line = plikOgloszeniaOld.readline()
    i = 0
    while line != '': 
        listaStarych.insert(i, line)
        line = plikOgloszeniaOld.readline()
        i = i + 1
    plikOgloszeniaOld.close()
#    print('Wczytano starych ogloszen: ' + str(i))


def zapiszOgloszeniaOld ():
    global ogloszeniaOld, lista
    i = 0
    plikOgloszeniaOld = open (ogloszeniaOld, 'w')
    for item in lista:
        plikOgloszeniaOld.write(item)
        i = i + 1
#    print ('Zapisano nowych rekordow: ' + str(i))
    plikOgloszeniaOld.close()


def sprawdzNowe ():
    global lista, listaStarych, listaNowych, ilosc_nowych
    i = 0
    for item in lista:
        try:
            index = listaStarych.index(item)
        except ValueError:
            listaNowych.insert(i,item)
            i = i+1

    ilosc_nowych  = i
    time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    print (time+ ' Nowe ogloszenia:' + str(i))
    for item2 in listaNowych:
        print (item2)


def sendMail ():
    global listaNowych

    //if len(listaNowych) < 1:
     //   return

    gmail_user = "raspberry.bencol@gmail.com"
    gmail_pwd = "Snowboard2"
    FROM = 'raspberry.bencol@gmail.com'
    TO = ['robert.gorecki@gmail.com'] #must be a list
    time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    SUBJECT = "Nowe Oglosznie z Otomoto z: " + time
    TEXT = "Lista Nowych Ogloszen: \n"
    i = 0    
    for item in listaNowych:
        i = i + 1
        TEXT = TEXT + str(i) + '. \t' + item + '\n'
    message = """\From: %s\nTo: %s\nSubject: %s\n\n%s""" % (FROM, ", ".join(TO), SUBJECT, TEXT)
    try:
        server = smtplib.SMTP("smtp.gmail.com", 587) #or port 465 doesn't seem to work!
        server.ehlo()
        server.starttls()
        server.login(gmail_user, gmail_pwd)
        server.sendmail(FROM, TO, message)
        server.close()
        print ('successfully sent the mail at : ' + time)
    except:
        print ('failed to send mail at: ' + time)

print ('=====================================================================================')
#wczytajOgloszenia()
#wczytajOgloszeniaOld()
#zapiszOgloszeniaOld()
#sprawdzNowe()
sendMail()
print ('=====================================================================================\n')

#!/usr/bin/env python

if __name__ == "__main__":

   import time
   import pigpio
   import vw
   import os

   RX=20
   TX=25

   BPS=2000

   pi = pigpio.pi() # Connect to local Pi.

   rx = vw.rx(pi, RX, BPS) # Specify Pi, rx GPIO, and baud.
   tx = vw.tx(pi, TX, BPS) # Specify Pi, tx GPIO, and baud.

   msg = 0

   start = time.time()

   while (time.time()-start) < 300:

      msg += 1

      #while not tx.ready():
      #   time.sleep(0.02)

      #time.sleep(0.1)

      #tx.put("{:04d}".format(msg))

     # while not tx.ready():
     #    time.sleep(0.02)

      #time.sleep(0.1)

      #tx.put("Hello World!")

      while rx.ready():
         print("".join(chr (c) for c in rx.get()))
         os.system("python3 /home/pi/MyPi/Pi/sendmail.py")

   rx.cancel() # Cancel Virtual Wire receiver.
   tx.cancel() # Cancel Virtual Wire transmitter.

   pi.stop() # Disconnect from local Pi.


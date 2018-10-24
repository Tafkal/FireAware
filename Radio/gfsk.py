import matplotlib as mp
import numpy as np
from rtlsdr import *
import matplotlib.pyplot as plt
import scipy.signal as signal
from scipy.signal import fir_filter_design as ffd
from scipy.signal import filter_design as ifd
from cmath import *
import socket
import time

TCP_IP = 'localhost'
TCP_PORT = 6969
MESSAGE = 'StartIT00'



#sdr = RtlSdr()
mp.use('Agg') #headless mode

F_station = int(869.49e6) #center freq
F_offset = 250000 #offset to avoid DC spike

Fc = F_station - F_offset #capture center freq
Fs = int(38400 * 30)
N = int(10240)

#configure device
# filter Configuration.
fS = Fs # Sampling rate.
fL = 200000  # Cutoff frequency.
Nl = 59  # Filter length, must be odd.

# Compute sinc filter.
h = np.sinc(2 * fL / fS * (np.arange(Nl) - (Nl - 1) / 2.))
# Apply window.
h *= np.blackman(Nl)

# Normalize to get unity gain.
h /= np.sum(h)


 #read samples
while(True):
    sdr = RtlSdr()
        #configure device


    sdr.sample_rate = Fs
    sdr.center_freq = Fc
    sdr.gain = 'auto'
    samples = sdr.read_samples(N)

     #clean up sdr
    sdr.close()
    del(sdr)


    #convert to numpy array
    x1 = np.array(samples).astype("complex64")
    # mix down with fc
    fc1 = np.exp(-1.0j*2.0*np.pi*F_offset/Fs * np.arange(len(x1)))

    x2 = x1 * fc1

    f_bw = 200000
    Inp1 = x2.real * np.cos(2.0*np.pi*Fc * np.arange(len(x2)))
    Qa1= -1*x2.imag * np.sin(2.0*np.pi*Fc * np.arange(len(x2)))

    Inp2 = np.convolve(Inp1,h)
    Qa2 = np.convolve(Qa1,h)

    delta_T = 155
    Inp2_delay = np.pad(Inp2, (155,0), 'constant', constant_values=(0,0))
    Qa2_delay = np.pad(Qa2, (155,0), 'constant', constant_values=(0,0))
    Inp3 = np.pad(Inp2, (0,155), 'constant', constant_values=(0,0))
    Qa3 = np.pad(Qa2,(0,155), 'constant', constant_values=(0,0))
    D = Qa3*Inp2_delay-Inp3*Qa2_delay
    y = []
    d = []
    k = 0
    for x in D:
        k+=1
        if x>0:
          y.append( 1 )
        else:
          y.append( 0 )
        if k == 30:
            k = 0
            p = np.mean(y)
            if p >= 0.5:
                d.append(1)
                y.clear()
            else:
                d.append(0)
                y.clear()
    y.clear()
    print(d)
    p = d.pop(0)
    t = 0
    for x in d:
        x=d.pop(0)
        if p == 0:
            if x == 1:
                t += 1
            else:
                t = 0
        elif  p == 1:
            if x == 0:
                t += 1
            else:
                t = 0
        p = x
        if t == 15:
            break
    if t == 15:
        print("FIRE FIRE FIRE FIRE FIRE FIRE FIRE")
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP, TCP_PORT))
        s.send(MESSAGE.encode())
        break
    else:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP, TCP_PORT))
        s.send("StartIT11".encode())
    time.sleep(0.100)

#modules
import matplotlib as mp
import numpy as np
from rtlsdr import *
import matplotlib.pyplot as plt


sdr = RtlSdr()

mp.use('Agg') #headless mode

F_station = int(869.49e6) #center freq
F_offset = 250000 #offset to avoid DC spike

Fc = F_station - F_offset #capture center freq
Fs = int(38400*30)
N = int(1024000*5)

#configure device
sdr.sample_rate = Fs
sdr.center_freq = Fc
sdr.gain = 'auto'

#read samples
samples = sdr.read_samples(N)

#clean up sdr
sdr.close()
del(sdr)

#conver to numpy array
x1 = np.array(samples).astype("complex64")

# mix down with fc
fc1 = np.exp(-1.0j*2.0*np.pi*F_offset/Fs * np.arange(len(x1)))

x2 = x1 * fc1

plt.specgram(x2, NFFT=2048, Fs=Fs)
plt.xlabel("Time (s)")
plt.ylabel("Frequency (Hz)")
plt.ylim(-Fs/2, Fs/2)
plt.xlim(0,len(x2)/Fs)
plt.ticklabel_format(style='plain', axis='y' )
plt.show()

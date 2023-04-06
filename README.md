# Apsa23 VST2 plugin

### English
This plugin was created as a project for the APSA (Architectures and Algorithms for Audio Signal Processing) exam of the master's degree in Sound and Performance Engineering at the University of Rome Tor Vergata.

The main idea is to create a plugin to creatively process signals from guitars, basses, synths, but also voices and complex buses.

The audio chain is as follows:

Fuzz -> Chorus -> Tone -> AutoTremolo -> Dry/Wet.

All effects are single knobs to make operation as simple and intuitive as possible.

### Tone
A simple control to emphasize low or high frequencies.
Processing is in parallel, and the user mixes the clean signal with either its own version with a boost in the bass or a version with a boost in the treble. Both boosts are achieved by two shelf filters, both with 750 Hz cutoff frequency and +12dB gain.

If the control is at 50% the effect is in bypass.
From 50% to 100% the high frequencies are progressively emphasized. 
From 50% to 0% the low frequencies are progressively emphasized.


### Fuzz
The effect is achieved by a variable gain stage up to 20 dB, followed by two distortion stages.
The first emulates a diode clipper with the following transfer function:

$$ y(n) = x + 2.5 x^3$$

The second, on the other hand, achieves soft clipping by approximation of the hyperbolic tangent:

$$ y(n)= \begin{cases} 
1 & \mbox{se }\quad x(n) \ge 3 \\
\frac{27x+x^3}{27+9x^2} & \mbox{se }\quad -3 \le x(n) \lt 3 \\
-1 & \mbox{se }\quad x(n) \lt -3 \\
\end{cases}$$


### Chorus
The chorus was implemented using a delay line with one write index and 4 read indexes.
The delays of the read indices with respect to the write index were modulated by a single unipolar LFO with a sine waveform with a constant frequency of 0.5 Hz.
Each read index has a different range of delays in which it varies.

Once the signals are obtained from the delay line, they are summed together, attenuated and then processed through a soft clipping.

### AutoTremolo
It is a simple tremolo where the user can change the depth parameter, while the frequency depends on the RMS value of the input signal.

### Dry/Wet
The dry/wet parameter is simply a linear interpolation between effected signal and clean signal.


---

### Italiano
Questo plugin è stato realizzato come progetto per l'esame di APSA (Architetture e Algoritmi per il Processamento del segnali Audio)  del master in Ingegneria del Suono e dello Spettacolo dell'Università di Roma Tor Vergata.

L'idea principale è quella di creare un plugin per processare in maniera creativa segnali di chitarre, bassi, synth ma anche voci e bus complessi.

La catena audio è la seguente:

Fuzz -> Chorus -> Tono -> AutoTremolo -> Dry/Wet

Tutti gli effetti sono single knob per rendere l'utilizzo più semplice ed intuitivo possibile.

### Tono
Un semplice controllo per enfatizzare le basse o le alte frequenze.
Il processamento è in parallelo e l'utente miscela il segnale pulito con una sua versione con un boost nei bassi o una versione con un boost nelle alte. Entrambi i boost sono ottenuti grazie a due filtri shelf, entrambi con frequenza di taglio a 750 Hz e un guadagno di +12dB.

Se il controllo si trova al 50% l'effetto è in bypass.
Dal 50% al 100% si enfatizzano progressivamente le alte frequenze. 
dal 50% al 0% si enfatizzano progressivamente le basse frequenze.

### Fuzz
L'effetto è ottenuto tramite uno stadio di guadagno variabile fino a 20 dB, seguito due stadi di distorsione.
Il primo emula un tosatore a diodi con la sequente funzione di trasferimento:

$$ y(n) = x +2.5x^3$$

Il secondo invece ottiene un soft clipping tramite approssimazione della tangente iperbolica:

$$ y(n)= \begin{cases} 
1 & \mbox{se }\quad x(n) \ge 3 \\
\frac{27x+x^3}{27+9x^2} & \mbox{se }\quad -3 \le x(n) \lt 3 \\
-1 & \mbox{se }\quad x(n) \lt -3 \\
\end{cases}$$

### Chorus
Il chorus è stato implementato utilizzando una linea di ritardo con un indice di scrittura e 4 indici di lettura.
I ritardi degli indici di lettura rispetto a quello di scrittura erano modulati da un solo LFO unipolare con forma d'onda sinusoidale a frequenza costante di 0.5 Hz.
Ogni indice di lettura ha un range diverso di ritardi nel quale varia.

Una volta ottenuti i segnali dalla linea di ritardo, questi vengono sommati tra loro, attenuati e poi processati tramite un soft clipping.

### AutoTremolo
È un semplice tremolo dove l'utente può modificare il parametro di profondità, mentre la frequenza dipende dal valore RMS del segnale in ingresso.

### Dry/Wet
Il parametro di dry/wet è semplicemente una interpolazione lineare tra segnale effettato e segnale pulito.






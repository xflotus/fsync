all : LinuxWatch.h libLinuxWatch.so

LinuxWatch.h :
	javah -cp ../bin watch.LinuxWatch
	mv watch_LinuxWatch.h LinuxWatch.h

libLinuxWatch.so : LinuxWatch.c
	gcc -o libLinuxWatch.so -fPIC -shared -I /usr/java/include -I /usr/java/include/linux LinuxWatch.c
	cp libLinuxWatch.so /usr/lib

clean:
	rm -fr *.so *~ LinuxWatch.h


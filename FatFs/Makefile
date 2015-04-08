# Add filename.o to include in compilation
OBJECTS=ff.o diskio.o fatfs_sd.o tm_stm32f4_spi.o tm_stm32f4_delay.o


%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

all: $(OBJECTS)

clean:
	rm -f *.o
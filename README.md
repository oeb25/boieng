# boieng
Boieng is a robot complete with concurrency, distribution and fault tolerance. Short for Boi Engenering 🕴

## Super fast up and running on OSX

```
$ ./run.sh
```

## Installing dependencies

### OSX

```bash
$ brew install platformio
```

### Linux

Idk...

## Running

``` bash
$ pio run -t upload && pio serialports monitor -p $(pio serialports list | grep usbmodem)
```

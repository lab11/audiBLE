AudiBLE App
=========

This app senses the sound level every second.

Programming
-----------

    make flash ID=c0:98:e5:30:00:01

Hardware
-------------

Squall with audiBLE

### Data Format

Manufacturer specific data in the advertisement:

| Index | Bytes        | Description                            |
| ----- | ------------ | -------------------------------------- |
| 0     | `0x07`       | Length                                 |
| 1     | `0xff`       | Header for manufacturer specific data  |
| 2     | `0x02e0`     | University of Michigan manufacturer ID |
| 4     | `0x1E`       | Service identifier for audiBLE data    |
| 5-8   | `0x00000000` | Sound Level               |



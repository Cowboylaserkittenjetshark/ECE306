# ECE 306 HW 2
### Ashton Henderlite
### Jan 30, 2026
---
1. 'display_line'
2. The character array representing the string '   NCSU   \0' (NCSU surounded by three spaces on either side, null terminated)
3. 0x008A8C
4. Pointer to 'char display[0]' is '0x00202C', pointer to 'char display[1]' is '0x00202E', pointer to 'char display[2]' is '0x002030', pointer to 'char display[3]' is '0x002032' 
5. '0x00202C' contains a pointer to '0x002000', '0x00202E' contains a pointer to '0x00200B', '0x002030' contains a pointer to '0x002016', '0x002032' contains a pointer to '0x002021'
6. '[0x20, 0x20, 0x20, 0x4E, 0x43, 0x53, 0x55, 0x20, 0x20, 0x20, 0x00]'
7. 3.3v * 49.1 mA = 162.03 mW delivered
   162.03 mW / 90% = 180.03 mW supplied
   180.03 / 2 = 90.02 per battery
   Graph gives 1 AH for ~90 mW
   1 AH / 0.0491 A = 20.37 hours
   20.37 hours * 60 = 1222 minutes
   1222 minutes / 20 minutes/day  = 61.1 days
8. It is located in '.bss' because it is unitialized

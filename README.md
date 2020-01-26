# E-Paper-Information-Board
Design Laboratory Project

# Log Horizon

1. Arduino IDE install.
2. STM32 core lib install.
3. Write test program (test_prog, Working Board.jpg).
4. Board Testing.
5. Prepare Grid and run tests (base_grid, Grid.jpg).
6. Created exel spreadsheet (https://docs.google.com/spreadsheets/d/1gxtl_GKmRAolRyfR_gdwDhk2ZIX_qNM08uLq0AaBJCs/edit?usp=sharing).
7. Reaserch (4h).
8. Doing Tests (network_test).
9. Find WiFi Shield Lib (https://github.com/stm32duino/WiFi-ISM43362-M3G-L44).
10. Doing Tests v2 (network_test2).
11. WiFi Shield works !!!!!!!!
12. WiFi Connection (No Encryption) (network_connect)
13. Wirte some google scrips code (https://script.google.com/d/1bj7HgN_XCmmqf7RyHpEqmJyIgPmN2LVnuz4rw4gfU4CADRtul6D5ehsa/edit)
14. Tring to Get that script (dont work for now).
15. Grid update.
16. Add NFC counter.
17. Tring to setup NFC.
18. NFC on board works.
19. Try seting all up with only nfc.
20. NFC on board works 
21. Getting info through NFC.
22. Update grid(final vercion.jpg).
23. Test all NFC commands.

# Video

[See video](https://youtu.be/UOiABJdIu4o)

# NFC commands

There are 4 commands that allow users to update display.

### S 123456 (Key: S (student) Number: 123456 (students ID))

This command increase NFC counter and refresh dispaly.

### N 2137 R (Key: N (teacher) Number: 2137 (teachers security code) Key: R (reset))

This command resets NFC counter and refresh dispaly.

### N 2137 H 1 MON 12:00 15:00 FRI 12:23 14:23 

(Key: N (teacher) Number: 2137 (teachers security code) Key: H (hours) Number: 1 (how many lines ther are max 3) Data: MON 12:00 15:00 FRI 12:23 14:23 (first normal hours for specified teacher then his/hers changed hours) )

This command sets hours for all teachers and refresh dispaly.

### N 2137 I 1 Testing

(Key: N (teacher) Number: 2137 (teachers security code) Key: I (news) Number: 1 (which lines ifno should be displayed max 7) 
Text: Testing (text that will de displayed))

This command sets specified text in specified line and refresh dispaly.

When u read NFC tag at any moment it will show all students ID that saves it.

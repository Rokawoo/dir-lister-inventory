#!/bin/bash

# Compile the program
gcc -o Inventory Inventory.c

# Test Scenario 1: Basic functionality
echo "Test Scenario 1: Basic functionality"
./Inventory <<EOF
ADD 928352:42:Handy Hoozit
ADD FS9283G:7:Salty Gismo
PRINT
REMOVE 928352:3
PRINT
QUIT
EOF

# Test Scenario 2: Unknown Item
echo "Test Scenario 2: Unknown Item"
./Inventory <<EOF
ADD 928352:42:Handy Hoozit
REMOVE FS9283G:5
QUIT
EOF

# Test Scenario 3: Insufficient Quantity
echo "Test Scenario 3: Insufficient Quantity"
./Inventory <<EOF
ADD 928352:10:Handy Hoozit
REMOVE 928352:15
QUIT
EOF

# Test Scenario 4: Whitespaces
echo "Test Scenario 4: Whitespaces"
./Inventory <<EOF
 ADD     F2983754:8:Bright Shiny Thing
ADD     DDE:5:Dull Fragile Doodad
 PRINT
 REMOVE     F2983754:2
PRINT
QUIT
EOF

# Test Scenario 5: Incorrect Syntax
echo "Test Scenario 5: Incorrect Syntax"
./Inventory <<EOF
ADD 928352
QUIT
EOF

# Test Scenario 6: Case Insensitivity
echo "Test Scenario 6: Case Insensitivity"
./Inventory <<EOF
ADD Item1:5:Test Item
PRINT
REMOVE ItEm1:3
PRINT
QUIT
EOF

# Test Scenario 7: Empty Inventory
echo "Test Scenario 7: Empty Inventory"
./Inventory <<EOF
PRINT
QUIT
EOF

# Test Scenario 8: Large Quantities
echo "Test Scenario 8: Large Quantities"
./Inventory <<EOF
ADD 123456789012345:10000:Large Quantity Item
PRINT
REMOVE 123456789012345:5000
PRINT
QUIT
EOF

# Test Scenario 9: Maximum String Lengths
echo "Test Scenario 9: Maximum String Lengths"
./Inventory <<EOF
ADD 123456789012345:1:Lorem ipsum dolor sit amet, consectetur adipiscing elit
PRINT
QUIT
EOF

# Clean up
rm Inventory

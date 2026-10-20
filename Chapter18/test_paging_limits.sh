
#!/bin/bash

# ==========================================
# CS5600 - Chapter 18
# Paging Simulator: Edge Case Experiments
# ==========================================


# SECTION 1: Define variables
# Store the simulator path and log filename.

SIMULATOR="./paging-linear-translate.py"
LOGFILE="paging_limits.log"


# SECTION 2: Initialize the log file
# Create the file or clear its previous contents.

> "$LOGFILE"


# SECTION 3: Experiment 1
# Physical memory smaller than virtual address space.

echo "===== EXPERIMENT 1 =====" | tee -a "$LOGFILE"
echo "Physical memory smaller than address space" | tee -a "$LOGFILE"

python3 "$SIMULATOR" -P 4k -a 4m -p 1m -v -c 2>&1 | tee -a "$LOGFILE"

echo "" | tee -a "$LOGFILE"


# SECTION 4: Experiment 2
# Page size larger than virtual address space.

echo "===== EXPERIMENT 2 =====" | tee -a "$LOGFILE"
echo "Page size larger than address space" | tee -a "$LOGFILE"

python3 "$SIMULATOR" -P 2k -a 1k -p 256m -v -c 2>&1 | tee -a "$LOGFILE"

echo "" | tee -a "$LOGFILE"


# SECTION 5: Experiment 3
# Physical memory equal to virtual address space.

echo "===== EXPERIMENT 3 =====" | tee -a "$LOGFILE"
echo "Physical memory equal to address space" | tee -a "$LOGFILE"

python3 "$SIMULATOR" -P 1k -a 256m -p 256m -v -c 2>&1 | tee -a "$LOGFILE"

echo "" | tee -a "$LOGFILE"


# SECTION 6: Experiment 4
# Page size equal to virtual address space.

echo "===== EXPERIMENT 4 =====" | tee -a "$LOGFILE"
echo "Page size equal to address space" | tee -a "$LOGFILE"

python3 "$SIMULATOR" -P 1k -a 1k -p 256m -v -c 2>&1 | tee -a "$LOGFILE"

echo "" | tee -a "$LOGFILE"


# SECTION 7: Completion message
# Confirm where the results were saved.

echo "All experiments completed."
echo "Results saved to $LOGFILE"


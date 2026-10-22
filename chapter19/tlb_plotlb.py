import re
import matplotlib.pyplot as plt

pages  = []
times = []

with open ("tlb.log", "r") as logfile:
    for line in logfile:
        # Extract the number of pages from lines such as:
        # pages: 8
        # Split at ":" and convert the result into an integer.
        if line.startswith("pages"):
            pages.append(int(line.split(":")[1]))

        if "average time per access:" in line.lower():
            # Regex searches for a number followed by "ns".
            # ([\d.]+) captures the numerical value.
            # \s* allows optional whitespace before "ns".

            match = re.search(r"([\d.]+)\s*ns", line)
            if match:
                times.append(float(match.group(1))) # if theres a match capture the decimal as a group convert to flot and append it to time in matplot


# pages supplies the X-axis values.
# times supplies the Y-axis values.
# marker="o" displays a circle at each measurement.
plt.plot(pages, times, marker="o")

# Use a base-2 logarithmic scale because our experiments
# test page counts in powers of two: 1, 2, 4, 8, 16, etc.
plt.xscale("log", base=2)

# Labels identify what each axis measures.
# The title describes the experiment.
# Grid lines make changes in access time easier to observe.

plt.xlabel("Number of pages")
plt.ylabel("Average Acess Time (ns)")
plt.title("TLB Performance vs. Number of Pages")
plt.grid(True)

# Save a high-resolution image for the homework report.
# Display the graph for visual inspection.
plt.savefig("tlb_performance.png", dpi=300)
plt.show()

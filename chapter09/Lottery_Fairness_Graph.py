"""Plot manually collected lottery results for a 50:25 ticket split.

Run with: python3 plot_manual_ticket_imbalance.py
Requires matplotlib. The PNG is saved beside this script.
"""

import matplotlib
from pathlib import Path

# Save directly to an image file; no graph window is needed.
matplotlib.use("Agg")
import matplotlib.pyplot as plt


# Each pair is (Job 0 finish time, Job 1 finish time), read from the screenshots.
# For each job length, the trials use different seeds.
results = {
    10: [(11, 20), (16, 20),(16, 20)],  # seeds 1, 1, 2
    20: [(33, 40), (30, 40),(33, 40)],  # seeds 1, 2
}


def fairness(finish_times):
    """Earlier finish divided by later finish; closer to 1 means closer finishes."""
    first = min(finish_times)
    last = max(finish_times)
    return first / last


job_lengths = sorted(results)
average_fairness = []

for length in job_lengths:
    # Compute one fairness value for each seed, then average those values.
    scores = [fairness(times) for times in results[length]]
    average_fairness.append(sum(scores) / len(scores))
    print(f"Length {length}: trial fairness={scores}; mean={average_fairness[-1]:.4f}")

plt.figure(figsize=(7, 4.5))
plt.plot(job_lengths, average_fairness, marker="o", linewidth=2)
plt.xticks(job_lengths)
plt.ylim(0.45, 1.02)
plt.xlabel("Job length (CPU time units)")
plt.ylabel("Mean fairness (earlier finish / later finish)")
plt.title("Lottery Fairness with 50:25 Tickets")
plt.grid(True, linestyle=":", alpha=0.6)
plt.tight_layout()
output_path = Path(__file__).with_name("ticket_imbalance_fairness.png")
plt.savefig(output_path, dpi=200)
print(f"Saved graph to {output_path}")


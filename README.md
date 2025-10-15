# Linux Kernel ReadAhead Prediction using Machine Learning

## 🌟 Project Overview

This project focuses on the intersection of **Operating Systems** and **Machine Learning** to improve system performance. The primary goal is to **determine and predict the optimal parameters for the Linux kernel's `readahead` mechanism** by analyzing kernel event traces under varying I/O workloads.

By understanding how different I/O patterns affect kernel events, a predictive model can be deployed to dynamically adjust `readahead`, leading to significant improvements in I/O throughput and reduced latency.

### 🎯 Key Objectives

  * Set up and execute various I/O benchmarks using the RocksDB Key-Value store.
  * Utilize **LTTng** (Linux Tracing Toolkit Next Generation) to capture detailed kernel events related to the Writeback and Page Cache mechanisms.
  * Perform extensive Feature Engineering on the raw trace data to extract meaningful kernel metrics.
  * Design, train, and validate a **Deep Learning model (ANN)** to classify the underlying I/O workload based on the extracted kernel features.

## 🛠️ Technologies and Tools

| Category | Tool / Language | Purpose |
| :--- | :--- | :--- |
| **Operating System** | Linux (Ubuntu) | Main environment for tracing and execution. |
| **Kernel Tracing** | LTTng (Linux Tracing Toolkit) | High-precision event tracing for Kernel Writeback events. |
| **Benchmarking** | RocksDB | Used to simulate four distinct I/O workloads (`readrandom`, `readseq`, `readreverse`, etc.). |
| **Machine Learning** | Python (TensorFlow/Keras) | Building and training the Sequential ANN model. |
| **Data Processing** | Python (Pandas, Numpy, Scikit-learn) | Trace parsing (`script.py`), Feature Engineering, and data preprocessing (`ml.py`). |
| **Conceptual Integration** | C++ | Demonstrating LTTng C API usage for in-kernel data extraction (`8.cpp`). |

## ⚙️ Methodology and Implementation

The project followed a three-phase approach:

### 1\. Data Tracing and Extraction

1.  **Workload Generation:** RocksDB was configured to run four I/O benchmarks, simulating different data access patterns (sequential, random, reverse).
2.  **Kernel Tracing:** An LTTng session was launched concurrently with the benchmarks. Key events traced included: `writeback_mark_inode_dirty`, `mm_vmscan_writepage`, and `writeback_pages_written`.
3.  **Trace Parsing:** Raw trace output (e.g., `readrandom.txt`) was parsed using Regular Expressions in `script.py` to extract event-specific fields.

### 2\. Feature Engineering and Preprocessing

**Key Files:** `script.py`, `ml.py`

From the raw LTTng events, **7 key features** were engineered for the model:

| Feature | Description |
| :--- | :--- |
| **Workload** | Target class label (e.g., `readrandom`). |
| **Inode Number (ino)** | Identifier of the file being accessed. |
| **State / Flags** | Kernel-level status and flags associated with the Writeback operation. |
| **Time Elapsed** | Delta time between consecutive kernel events. |
| **Cumulative Time** | Running sum of time elapsed, providing context of total time spent in a state. |
| **Transaction (Ops/Sec)** | Estimated rate of operations (calculated during data prep). |

  * **Preprocessing:** Data was normalized using **Z-score (StandardScaler)**.
  * **Dimensionality Reduction:** **t-SNE** was applied to visualize the clustered nature of the four workloads in a 2D space, confirming separability.

### 3\. Machine Learning Model

**Key Files:** `ml.py`, `OSLabProject.ipynb`

A **Sequential Artificial Neural Network (ANN)** was implemented using TensorFlow/Keras for multi-class classification (4 Workload types).

**Model Architecture:**

  * **Input Layer:** Matches the number of extracted features.
  * **Hidden Layers:** Multiple `Dense` layers with `ReLU` activation and **Dropout (0.5)** layers for regularization.
  * **Output Layer:** 4 units with `Softmax` activation for classification.
  * **Optimizer:** Adam with ExponentialDecay learning rate schedule.

**Performance Metrics:**

| Evaluation Metric | Value |
| :--- | :--- |
| **Accuracy** | **0.9626** |
| **F1-Score (Macro Avg)** | **0.9625** |

## 💡 Key Findings and Conclusion

The high classification accuracy demonstrates that **kernel event characteristics, specifically the combination of `flags`, `transaction` rate, and `cumulative time`, are highly effective predictors of the underlying I/O workload**.

By accurately classifying the workload at any given moment, the system gains the necessary information to make a well-informed prediction for the optimal `readahead` value, which is crucial for maximizing I/O performance in complex Linux systems. This model serves as a proof-of-concept for a potential dynamic `readahead` policy implemented within the kernel.

## 📁 Repository Structure

```
.
├── 8.cpp                    # Conceptual C++ code for LTTng C-API usage (data extraction from trace).
├── ml.py                    # Python script containing the ANN model and all ML training/evaluation.
├── OSLabProject.ipynb       # Jupyter Notebook version of the ML analysis and visualization.
├── OsLabProject.pdf         # Final project report (in Persian).
├── script.py                # Python script for initial parsing of raw LTTng trace files.
├── readrandom.txt           # Raw LTTng trace data for 'readrandom' workload.
├── readreverse.txt          # Raw LTTng trace data for 'readreverse' workload.
└── readseq.txt              # Raw LTTng trace data for 'readseq' workload.
```

## 🧑‍🤝‍🧑 Contributors

  * [Mahdis Sepahvand]
  * [Mobina Shahbazi, Babak Fathi, Rasa Bakhtiyari]
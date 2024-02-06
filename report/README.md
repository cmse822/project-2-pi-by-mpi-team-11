# Project 2 Report
## By Max Brombach, Ahmad Hejasebazzi, and Nima Mirzaeian

## Part 1: Warmup Exercises

2.18)

2.19)

2.21)

2.22)

2.23)

2.27)

## Part 3: MPI Basics

The output printed the first print statement four times, then the middle statement four times, then the final statement four times despite the first and final statements being before and after the `MPI_Init()` and `MPI_Finalize()` statements, respectively. This is due to the fact that MPI runs all "ranks" in parallel from the beginning of the code to the end, not from the `MPI_Init()` to the `MPI_Finalize()`.

2.3)

2.4)

2.5)

## Part 4: Eat Some Pi



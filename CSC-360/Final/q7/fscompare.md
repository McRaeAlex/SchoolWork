# Question 6 Part 1

`Question: How do file system metadata and infrastructure differ between the three file systems we coved in this course, UFS, FFS, and LFS?`

## UFS

* has all the inodes and meta data in one location, which can slow it down as it has to seek from the inode area to the data area.
* has a max number of inodes are determined by the inode section of disk
* Simple

## FFS

The fast filesystem partitions (cylander group) the disk into sections. Each section was like its own mini unix file system to reduce time to seek to the inode section.

* Inode section at beginning of each partition with metadata about that partition
* Generally faster than UFS

## LFS

The log file system works by writing a log and recreating the file from that log

* Really fast to write
* Can be slow to reconstruct the file system from the log
* Recovery from crashes are easier as you can just read to the last consistent part of the log
* Inodes are scattered throughout the filesystem log
* When data is low blocks near the head must be reclaimed and reconstructed





# Question 6 part 2

Inode of file1.txt
```
8394354
```

Inode of file2.txt
```
8394354
```

Question: `How does the metadata support these files, and can they be updated and be deleted independently?`

Metadata supports this relationship by allowing multiple directory entires to point to the same inode and keep track of how many entries are pointing to it.

Updates to file1.txt are reflected in file2.txt and the reverse is also true.
Deleting file1.txt does not delete file2.txt and the reverse is also true.

The reason for this is because file1.txt and file2.txt are just a directory entries which point
to an inode. The inode has some counter on it to know how many directory entires are pointing to it
and only gets deleted when all are [1]. Since both file1.txt and file2.txt are pointing to the same inode
it makes sense changes in one are reflected in the other.

I tested [1] by deleting them both and recreating the file1.txt which now has an 
inode value of `8394355`. I expect the old inode will be garbage collected or its currently in use somewhere else in the system.


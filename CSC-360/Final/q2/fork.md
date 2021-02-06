# Question 2

The code given starts 8 processes (including the inital process)

The inital process will call fork 3 times (a, b, c).

a will call fork 2 more times (a1, a2)

b will call fork 1 more time (b1)

c will not call fork again

a1 will then call fork 1 more time (a1i)

a2 will not call fork again

1 (inital) + 3 (a, b, c) + 3 (a1, a2, b1) + 1 (a1i) = 8
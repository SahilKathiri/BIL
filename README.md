# BIL (BrainF**k Interpreted as a Language)

BIL is an interpreter for BrainF*\*k written in C. It has all the operations of BrainF*\*k with some _magic_ functions.

BrainF*\*k Operators 
---
Brainf*\*k assumes you have an array of at least 33,000 cells that can have a 8-bit character. 

| Operator        | Function           |
|:---------------:|:-------------      |
|   +   | Increment value at current cell |
|   -   | Decrement value at current cell |
|   <   | Move left one cell |
|   >   | Move right one cell |
|   .   | Print value of current cell in ASCII |
|   ,   | Get user input for current cell |
|   [   | If value of current cell is non-zero then execute loop, else skip to matching ']' |
|   ]   | Jump to matching '[' |

_Magic_ Functions
---
BIL also has some additional _magic_ functions. They take an integer argument or the ip as the default argument.

|   Function    |   Alias   |   Argument    |   Description |
|:----:|:---:|:---:|---|
|   %%help   |   %%h    |   -    |  Display Help     |
|   %%print   | %%p      |   n -> cell index    |   Print     |
|   %%seek   |   %%s    |   n -> cell index    |   Go to cell index n    |
|   %%peek   |  %%pk     |   n -> cell index    |   View ascii of value ofindex n    |
|   %%peekd   | %%pd      |   n -> cell index    |  View decimal value of index n    |
|   %%peekh   | %%ph      |    n -> cell index   |  View hex value of index n     |
|   %%inc   |   %%i    |   n -> value    |     Increment value at current index by n  |
|   %%dec   |   %%d    |   n -> value    |     Decrement value at current index by n  |
|   %%dump   |   %%dd    |   n -> cell index    |   View decimal dump around index n    |
|   %%dumph   | %%dh      |  n -> cell index     |  View hex dump around index n     |



##### Library Used
MPC - Multi Parser Combinator - <https://github.com/orangeduck/mpc>   
Licensed Under BSD  
Copyright (c) 2013, Daniel Holden All rights reserved.

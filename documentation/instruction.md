# Instructions

if we reduce the register count to 255, then we only need 1 byte to encode the register number.
so that's 3/7 bytes per instruction which leavers 4 bytes for the opcode and format. which if 
we expand both to 8 bits, thats 5/7 bytes per instruction for an 8 byte instruction.
we could have a 16 bit immediate, or two more register arguments in the remaining 2 bytes.

4 registers in an instruction lets you specify 3 source registers and 1 destination register.
you could encode clamp (a, b, c)
I don't know how useful this is.
you could have 2 source registers and 2 destination registers, giving an easy way to return 
the result of quotient and remainder of division at the same time.

a general purpose language wants to support an arbitrary number of return values. for instance 
returning a tuple, and having a syntax to decompose that into multiple names. This can be done 
with a single destination register, and then mapping the elements of that tuple into local variables.
so the 2 destination registers does not help there.

an instruction like getElementPtr(base, index, scale) could be encoded with 3 source registers and 1 destination register.
or two source registers and an immediate.

however this instruction can also be computed with multiple instructions. 
```
	mul <t0>, <index>, <scale>
	add <t1>, <base>, <t0>
```
so the 4 register instruction does not add any new capabilities, it just makes some instructions more compact. 




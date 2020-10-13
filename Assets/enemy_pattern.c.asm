
# Assignment Statement 
	PUSH 2

# RAND FUNC (builtin)
	PUSH 16
	RAND 
	ADD
	SAVE %max_moves

# Assignment Statement 
	PUSH 1
	SAVE %spin

# Assignment Statement 
	PUSH 1
	SAVE %dir

# WHILE 
:LBL_0024
	PUSH 1
	BRF @LBL_0025

# WHILE BLOCK
# BEGIN BLOCK

# Assignment Statement 
	LOAD %max_moves
	SAVE %moves

# WHILE 
:LBL_0026
	LOAD %moves
	PUSH 0
	GT
	BRF @LBL_0027

# WHILE BLOCK
# BEGIN BLOCK

# Assignment Statement 
	LOAD %moves
	PUSH 1
	SUB
	SAVE %moves

# MOVE FUNC (builtin)
	LOAD %dir
	MOVE 

# END BLOCK
	JMP @LBL_0026
:LBL_0027

# Assignment Statement 
	LOAD %dir
	LOAD %spin
	ADD
	SAVE %dir

# END BLOCK
	JMP @LBL_0024
:LBL_0025

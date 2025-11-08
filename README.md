# mkm-editor

## How to compile and run:
```bash
gcc -g /workspaces/mkm-editor/main.c -o /workspaces/mkm-editor/main
/workspaces/mkm-editor/main
```

## First Test - File Read - Writing Function
Ran
```bash
make
echo -e "alpha\nbeta\r\ngamma" > sample.txt
./build/mkm sample.txt
```
and it gave me first the 200 bytes of the file sucessfully.

`malloc` allocates memory and returns a pointer to it.
On `calloc` the memory is not initialized.
`fseek` used for setting the file pointer at the specific position in the file.
`ftell` used to obtain the current file position of a stream.
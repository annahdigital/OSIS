.model small

.stack 100h
.386

.data
    DTA db 128h dup(0)  
    max   db   80      
    len   db   0 
    inputData   db   254, 0, 255 DUP (0) 
    outputData   db   254, 0, 255 DUP (0) 
    handleInput    dw 1 
    handleOutput    dw 1 
    buffer   db   255, 0, 256 DUP (0) 
    bufferencryptingProc   db   255, 0, 256 DUP (0) 
    inputMessage      db   "Enter input file: $" 
    outputMessage      db   "Enter output file: $" 
    inputFileMes db "There is no file like this input file.$"
    outputFileMess db "There is no file like this output file.$"
    finishingMessage db "encryptingProcing is done!$"
    foundErrorCaseMessage db "Unexpected foundErrorCase!!! Sorry...$"
    filesD db '*.*', 0 
    XorKey db 'Borisova'

.code

printingBufferDataProc proc
push bx
push ax
push cx
    mov bx, offset buffer
    add bx,ax             
    mov byte ptr [bx],'$'     
 
    mov ah,9
    mov dx, offset buffer
    int 21h                
 
    mov cx,256
    call newlineProcedure   
pop cx
pop ax
pop bx
    ret
printingBufferDataProc endp

printingfoundErrorCaseMessageProc proc  
    mov dx,offset foundErrorCaseMessage
    mov ah,9
    int 21h
    call newlineProcedure
    mov ah, 02h
    mov dl, al
    add dl, '0'
    int 21h
    ret
printingfoundErrorCaseMessageProc endp

newlineProcedure proc
    push dx
    push ax
    mov dx,13 
    mov ah,2
    int 21h  
    mov dx,10
    mov ah,2
    int 21h
    pop ax
    pop dx
    ret
 newlineProcedure endp

enteringOutputData   proc  
    push dx
    push ax
    lea   dx, outputMessage   
    mov   ah, 9 
    int   21h
    mov dx, offset outputData   
    mov AH, 0ah 
    int 21h
     
    mov al,10
    int 29h
    pop ax
    pop dx
    ret 
enteringOutputData   endp  

enteringInputData   proc  
    push dx
    push ax
    lea   dx, inputMessage   
    mov   ah, 9 
    int   21h
    mov dx, offset inputData   
    mov AH, 0ah 
    int 21h
     
    mov al,10
    int 29h
    pop ax
    pop dx
    ret 
enteringInputData   endp  

encryptingProc proc
    push cx
    push dx
    push bx
    
    lea si, buffer
    lea di, bufferencryptingProc
    
    xor cx, cx
    cmp ax, 0
    je skipping
    mov dx, ax
    mov bx, offset XorKey
    push ax
    cld
    ecryptloop:
    lodsb
    xor al, [bx]
    stosb
    inc cx
    inc bx
    cmp cx, 9
    jne key_skip
    sub bx, 9
    sub cx, 9
    sub dx, 9
    key_skip:
    cmp cx, dx
    jne ecryptloop
    
    pop ax
    skipping:
    pop bx
    pop dx
    pop cx
    ret
encryptingProc endp


start:
    mov ax, @Data
    mov ds, ax
    MOV ES, AX 
    
    CALL enteringInputData 
    CALL newlineProcedure
    CALL enteringOutputData
    CALL newlineProcedure
    
    mov dx,offset DTA
    mov ah,1Ah
    int 21h 
    lea dx, filesD
    xor cx, cx
    mov ah,4Eh

    int 21h
    jc  quit

filenameChecking:
    xor bx, bx
    cld
    mov ESI, offset inputData+2
    mov EDI, offset DTA + 30
    mov ECX, 6 
    repe cmpsb 
    jne not_equal_1
    
    pop bx
    cmp bx, 1
    je skip_1
    mov bx, 1
    skip_1:
    push bx
    
    not_equal_1:
    cld
    mov ESI, offset outputData+2
    mov EDI, offset DTA + 30
    mov ECX, 6 
    repe cmpsb 
    jne not_equal_2
    
    mov dx,offset outputFileMess
    mov ah,9
    int 21h
    call newlineProcedure
    
    not_equal_2:
next_char:
    lodsb
    test al, al
    jnz next_char

    mov dx, offset filesD
    xor cx, cx
    mov ah, 4fh
    int 21h
    jnc filenameChecking
    pop bx
    cmp bx, 1
    je existingInput
    
    mov dx,offset inputFileMes
    mov ah,9
    int 21h
    call newlineProcedure
    jmp quit
    
    existingInput: 
    xor cx, cx
    lea bx, outputData + 2
    mov cl, outputData + 1
    add bx, cx
    mov byte ptr [bx], '$'
    mov ah,3Ch              
    mov dx, offset outputData + 2
    xor cx,cx               
    int 21h                 
    jnc foundErrorCaseSkipping                  
    call printingfoundErrorCaseMessageProc          
    jmp quit        

foundErrorCaseSkipping:
    mov [handleOutput],ax         
    
    xor cx, cx
    lea bx, inputData + 2
    mov cl, inputData + 1
    add bx, cx
    mov byte ptr [bx], '$'
    mov ah, 3dh
    mov al, 0 
    mov dx, offset inputData + 2
    int 21h
    jc foundErrorCase
    mov [handleInput], ax
    
readingF:
mov bx, [handleInput]           
    mov ah, 3Fh              
    mov dx, offset buffer           
    mov cx, 256              
    int 21h                 
    jnc foundErrorCaseSkipping2    
    call printingfoundErrorCaseMessageProc          
    jmp quit 

foundErrorCaseSkipping2:                  
    call encryptingProc
    
    push ax
    
    mov bx, [handleOutput]
    mov dx, offset bufferencryptingProc
    mov cx, ax
    mov ah, 40h
    int 21h
    
    pop ax
    cmp ax, 0
    jne readingF
    jmp closingFiles

foundErrorCase:
    mov dx,offset foundErrorCaseMessage
    mov ah,9
    int 21h
    call newlineProcedure
    mov ah, 02h
    mov dl, al
    add dl, '0'
    int 21h
    jmp quit
    
closingFiles:
    mov ah,3Eh            
    mov bx,[handleInput]         
    int 21h                 
    jnc continue_close                
    call foundErrorCase     
       
continue_close:
    mov ah,3Eh              
    mov bx,[handleOutput]         
    int 21h 
    lea   dx, finishingMessage   
    mov   ah, 9 
    int   21h    
    jnc quit                
    call foundErrorCase       
quit:   
    mov ax, 4c00h
    int 21h
end start
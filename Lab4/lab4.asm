.model small
.stack 1000
.data
        old dd 0 
        s db "abc", '$'
.code
.486
        count db 0 
        cnt db 0
 
digit_output macro
	local putting_1
	local putting_2
	local ex_n
 
	push 	ax
	push	cx
	push 	-1	
	mov 	cx, 10	

putting_1:
	xor 	dx, dx
	mov 	ah, 0                   
	div 	cl	
	mov 	dl, ah	
	push 	dx	
	cmp	al, 0	
	jne	putting_1	
	mov	ah, 2h

putting_2:	pop	dx	
	cmp	dx, -1	
	je	ex_n
	add	dl, '0'
	int	21h	
	jmp	putting_2	

ex_n:
	mov	dl, ' ' 
	int	21h
	pop	cx
	pop ax

endm


new_handle proc far      

        push ds si es di dx cx bx ax 
        
        xor ax, ax 
        xor dx, dx
        in  ax, 60h        
        cmp ax, 38h
        je switch

        mov dl, count
        cmp dl, 1
        je old_handler

        cmp  al, 39h        
        mov dl, ' '
        je new_handler

        jmp old_handler

        switch:
        	mov al, cnt
        	digit_output
        	jmp exit
        	
        new_handler:
        		je exit
			     
        old_handler: 
                pop ax bx cx dx di es si ds                       
                jmp dword ptr cs:old        
                
        exit:
                xor ax, ax
                mov al, 20h
                out 20h, al 
                pop ax bx cx dx di es si ds 
        iret

new_handle endp
 
 
ending_out:
 
start:
        mov ax, @data
        mov ds, ax
        
        cli 
        pushf 
        push 0        
        pop ds
        mov eax, ds:[09h*4] 
        mov cs:[old], eax 
        
        mov ax, cs
        shl eax, 16
        mov ax, offset new_handle
        mov ds:[09h*4], eax 
        sti 
        
        xor ax, ax
        mov ah, 31h
        MOV DX, (ending_out - @code + 10FH) / 16 
        INT 27H 

end start
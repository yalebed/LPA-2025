.586P
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "D:\bstu\3sem\lpa2\lpaa\lpa\LPA-2025\Debug\LIB.lib"
ExitProcess PROTO : DWORD

SetConsoleCP PROTO : DWORD

SetConsoleOutputCP PROTO : DWORD

strcpr PROTO : DWORD, : DWORD 

atoli PROTO : DWORD 

writestr PROTO : DWORD 

writeint PROTO : SDWORD 

writebool PROTO : BYTE 

writechar PROTO : BYTE 

.stack 4096

.const
L0 SDWORD 1 ; int (4 bytes)
L2 DB "тест функции getAnd", 0 ; str
L3 SDWORD 4 ; int (4 bytes)
L4 SDWORD 5 ; int (4 bytes)
L6 DB "тест функции getMul", 0 ; str
L7 SDWORD 200 ; int (4 bytes)
L8 SDWORD 200 ; int (4 bytes)
L10 DB "тест операций (логических, унарных, арифметических)", 0 ; str
L11 SDWORD 3 ; int (4 bytes)
L12 SDWORD 5 ; int (4 bytes)
L13 SDWORD 11 ; int (4 bytes)
L14 SDWORD 5 ; int (4 bytes)
L15 SDWORD 9 ; int (4 bytes)
L16 SDWORD 100 ; int (4 bytes)
L17 SDWORD 2 ; int (4 bytes)
L19 DB "тест функции сравнения строк стандартной библиотеки", 0 ; str
L21 DB "aq", 0 ; str
L23 DB "a", 0 ; str
L25 DB "тест функции преобразования строки в число стандартной библиотеки", 0 ; str
L27 DB "123", 0 ; str
L29 DB "тест типы данных", 0 ; str
L30 SDWORD 11 ; int (4 bytes)
C32 BYTE 81 ; symb
L33 DB "it`s line", 0 ; str
L35 DB "просто текст", 0 ; str
L37 DB "тест условного оператора switch-is-any", 0 ; str
L38 SDWORD 1 ; int (4 bytes)
L39 SDWORD 2 ; int (4 bytes)
L41 DB "two", 0 ; str
L42 SDWORD 1 ; int (4 bytes)
L44 DB "one", 0 ; str
L45 SDWORD 3 ; int (4 bytes)
L47 DB "three", 0 ; str
L49 DB "any", 0 ; str
L51 DB "тест условного оператора when-other", 0 ; str
L52 SDWORD 0 ; int (4 bytes)
L53 SDWORD 2 ; int (4 bytes)
L54 SDWORD 1 ; int (4 bytes)
L56 DB "тест вложенных условных операторов", 0 ; str
L57 SDWORD 2 ; int (4 bytes)
L58 SDWORD 2 ; int (4 bytes)
L60 DB "val is 2", 0 ; str
L61 SDWORD 5 ; int (4 bytes)
L63 DB "and bz > 5", 0 ; str
L65 DB "other", 0 ; str
L67 DB "тест сложного выражения", 0 ; str
L68 SDWORD 1 ; int (4 bytes)
L69 SDWORD 2 ; int (4 bytes)
L70 SDWORD 2 ; int (4 bytes)
L71 SDWORD 2 ; int (4 bytes)
L72 SDWORD 2 ; int (4 bytes)

.data
yesand SDWORD 0 ; int
iv SDWORD 0 ; int
yesor SDWORD 0 ; int
inversion SDWORD 0 ; int
az SDWORD 0 ; int
bz SDWORD 0 ; int
incr SDWORD 0 ; int
decr SDWORD 0 ; int
w SDWORD 0 ; int
s1 DWORD 0 ; str
s2 DWORD 0 ; str
result SDWORD 0 ; int
s3 DWORD 0 ; str
res SDWORD 0 ; int
bb SDWORD 0 ; int
symbol BYTE 0 ; symb
line DWORD 0 ; str
sw SDWORD 0 ; int
flag BYTE 0 ; boolean
val SDWORD 0 ; int
clozh SDWORD 0 ; int

.code

FgetAnd PROC uses ebx ecx edi esi, a : SBYTE, b : SBYTE
; response
movsx eax, a
mov a, al
movzx eax, a
; Logical AND
mov al, a
and al, b
mov b, al
ret
FgetAnd ENDP


FgetMul PROC uses ebx ecx edi esi, a : DWORD, b : DWORD
; response
mov eax, a
; Multiplication
mov eax, a
imul eax, b
mov b, eax
ret
FgetMul ENDP

main PROC
Invoke SetConsoleCP, 1251
Invoke SetConsoleOutputCP, 1251

push offset L2
CALL writestr

; string #11 : ;metka
push L3
push L4
call FgetAnd
push eax
pop eax
mov yesand, eax

mov eax, yesand
push eax
CALL writeint

push offset L6
CALL writestr

; string #15 : ;metka
push L7
push L8
call FgetMul
push eax
pop eax
mov iv, eax

mov eax, iv
push eax
CALL writeint

push offset L10
CALL writestr

; string #19 : ;metka
push L11
push L12
pop ebx
pop eax
or eax, ebx
push eax
pop eax
mov yesor, eax

; string #20 : ;metka
push L13
pop eax
not eax
push eax
pop eax
mov inversion, eax

; string #22 : ;metka
mov eax, L14
mov az, eax

; string #23 : ;metka
mov eax, L15
mov bz, eax

; string #24 : ;metka
push az
pop eax
inc eax
mov byte ptr [az], al
push eax
pop eax
mov incr, eax

; string #25 : ;metka
push bz
pop eax
mov ebx, eax
dec eax
mov byte ptr [bz], al
mov eax, ebx
push eax
pop eax
mov decr, eax

; string #27 : ;metka
push L16
push L17
pop ebx
pop eax
cdq
idiv ebx
push eax
pop eax
mov w, eax

mov eax, yesor
push eax
CALL writeint

mov eax, inversion
push eax
CALL writeint

mov eax, incr
push eax
CALL writeint

mov eax, decr
push eax
CALL writeint

mov eax, w
push eax
CALL writeint

push offset L19
CALL writestr

; string #36 : ;metka
mov eax, OFFSET L21
mov [s1], eax

; string #37 : ;metka
mov eax, OFFSET L23
mov [s2], eax

; string #38 : ;metka
push s1
push s2
call strcpr
push eax
pop eax
mov result, eax

mov eax, result
push eax
CALL writeint

push offset L25
CALL writestr

; string #42 : ;metka
mov eax, OFFSET L27
mov [s3], eax

; string #43 : ;metka
push s3
call atoli
push eax
pop eax
mov res, eax

mov eax, res
push eax
CALL writeint

push offset L29
CALL writestr

; string #47 : ;metka
mov eax, L30
mov bb, eax

mov eax, bb
push eax
CALL writeint

; string #49 : ;metka
movsx eax, C32
mov symbol, al

; string #50 : ;metka
mov eax, OFFSET L33
mov [line], eax
push eax
movzx eax, symbol
push eax
CALL writechar
pop eax


push line
CALL writestr

push offset L35
CALL writestr

push offset L37
CALL writestr

; string #56 : ;metka
mov eax, L38
mov sw, eax

; SWITCH START ID: 1 ---
movzx eax, byte ptr [sw]
; is Check 2
cmp eax, 2
jne Switch_1_Next_0

push offset L41
CALL writestr
jmp Switch_1_EXIT
Switch_1_Next_0:
; is Check 1
cmp eax, 1
jne Switch_1_Next_1

push offset L44
CALL writestr
jmp Switch_1_EXIT
Switch_1_Next_1:
; is Check 3
cmp eax, 3
jne Switch_1_Next_2

push offset L47
CALL writestr
jmp Switch_1_EXIT
Switch_1_Next_2:
; any is

push offset L49
CALL writestr
jmp Switch_1_EXIT
Switch_1_EXIT:
; SWITCH END

push offset L51
CALL writestr

; string #65 : ;metka
mov eax, L52
mov flag, al

; --- IF BEGIN 2 ---
mov eax, inversion
mov ebx, L53
cmp eax, ebx
jg If_True_2
jmp If_Else_2
If_True_2:

; string #67 : ;metka
mov eax, L54
mov flag, al
push eax
movzx eax, flag
push eax
CALL writebool
pop eax

jmp If_Exit_2
If_Else_2:
push eax
movzx eax, flag
push eax
CALL writebool
pop eax

If_Exit_2:
; --- IF END 2 ---

push offset L56
CALL writestr

; string #75 : ;metka
mov eax, L57
mov val, eax

; SWITCH START ID: 4 ---
mov eax, val
; is Check 2
cmp eax, 2
jne Switch_4_Next_0

push offset L60
CALL writestr

; --- IF BEGIN 5 ---
mov eax, bz
mov ebx, L61
cmp eax, ebx
jg If_True_5
jmp If_Else_5
If_True_5:

push offset L63
CALL writestr
jmp If_Exit_5
If_Else_5:
If_Exit_5:
; --- IF END 5 ---
jmp Switch_4_EXIT
Switch_4_Next_0:
; any is

push offset L65
CALL writestr
jmp Switch_4_EXIT
Switch_4_EXIT:
; SWITCH END

push offset L67
CALL writestr

; string #89 : ;metka
push L68
push L69
pop ebx
pop eax
add eax, ebx
push eax
push L70
pop ebx
pop eax
imul eax, ebx
push eax
push L71
push L72
pop ebx
pop eax
imul eax, ebx
push eax
pop ebx
pop eax
sub eax, ebx
push eax
push val
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov clozh, eax

mov eax, clozh
push eax
CALL writeint
push -1
call ExitProcess
main ENDP
end main

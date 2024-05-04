// RPNConverterAsm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main() {
	// SaveESP
	int saveESP;
	_asm {
		mov saveESP, esp;
	}

	// Change Language
	const char* language = "Hun";
	_asm {
		push language;
		push LC_ALL;
		call setlocale;
	}

	// Infix input
	char infix[255];
	const char* askInfix = "Kérem, adja meg az infix kifejezést: ";
	const char* strFormat = "%[^\n]s";
	_asm {
		push askInfix;
		call dword ptr printf;
		add esp, 4;

		lea eax, infix;
		push eax;
		push strFormat;
		call scanf;
	}

	// remove Spaces
	_asm {
		lea esi, infix;
		mov edi, esi;
		jmp startLoop;

	loopBegin:
		mov al, [esi];
		cmp al, ' ';
		je skipChar;

		mov[edi], al;
		inc edi;

	skipChar:
		inc esi;

	startLoop:
		cmp byte ptr[esi], 0;
		jne loopBegin;

		mov byte ptr[edi], 0;
	}

	// Infix string size
	int strSize;
	char empty = '\0';
	const char* sizeMSG = "Infix string mérete: %d\n";
	_asm {
		mov strSize, 0;
		lea esi, infix;
	sizeLoop2:
		lea edi, empty;
		inc strSize;
		mov eax, 0;
		LODSB; //Karakterérték betöltése [ESI] helyről AL-be, majd 1-el növelés
		DEC esi;
		CMPSB; //[ESI] összehasonlítás [EDI]-vel, állapotjelzők beállítása. ESI, EDI növelése 1 - el.
		jnz sizeLoop2;
		dec strSize; //'\0'-t nem számoljuk bele a karakterektömb méretébe.
	}

	_asm {
		push strSize;
		push sizeMSG;
		call dword ptr printf;
		add esi, 4;
	}

	// Print Infix string
	const char* infixWithoutSpace = "Beolvasott Infix string: %s\n";
	_asm {
		lea esi, infix;
		push esi;
		push infixWithoutSpace;
		call printf;
		add esi, 4;
	}

	// IsOperand
	_asm {
		jmp skipIsOperand;
	isOperand:
		mov edx, [esp + 4];
		cmp dl, 48;
		jl not_operand;
		cmp dl, 57;
		jg not_operand;
		mov edx, 1;
		jmp end_isOperand;
	not_operand:
		mov edx, 0;
	end_isOperand:
		ret;
	skipIsOperand:
	}

	// isOperator
	_asm {
		jmp skipIsOperator;
	isOperator:
		mov edx, [esp + 4];
		cmp dl, '+';
		je returnTrue;
		cmp dl, '-';
		je returnTrue;
		cmp dl, '*';
		je returnTrue;
		cmp dl, '/';
		je returnTrue;
		cmp dl, '%';
		je returnTrue;
		cmp dl, '=';
		je returnTrue;
		mov edx, 0;
		ret;
	returnTrue:
		mov edx, 1;
		ret;
	skipIsOperator:
	}

	// precedence
	_asm {
		jmp skipPrecedence;
	precedence:
		mov edx, [esp + 4];
		cmp dl, '+';
		je returnOne;
		cmp dl, '-';
		je returnOne;
		cmp dl, '*';
		je returnTwo;
		cmp dl, '/';
		je returnTwo;
		cmp dl, '%';
		je returnTwo;
		cmp dl, '=';
		je returnThree;
		mov edx, 0;
		ret;
	returnOne:
		mov edx, 1;
		ret;
	returnTwo:
		mov edx, 2;
		ret;
	returnThree:
		mov edx, 3;
		ret;
	skipPrecedence:
	}

	// Infix to Postfix
	char* postfix = new char[strSize * 2 + 1];
	int index;
	int i;
	char c;
	const char* errorOpening = "Hiba: Nyitó zárójel hiányzik!";
	const char* errorOperator = "Hiba: Érvénytelen operátor vagy operandus!";
	const char* errorClosing = "Hiba: Bezáró zárójel hiányzik!";
	const char* errorEmpty = "Hiba: Zárójel tartalma hiányzik!";
	const char* errorConsecutive = "Hiba: Egymást követő operátorok találkoztak!";
	const char* errorWithoutOperand = "Hiba: Operandus nélküli operátor!";
	const char* errorZero = "Hiba: Osztás nullával!";

	bool operandEncountered;
	bool prevWasOperator;
	const char* test = "asd";
	int emptyCounter;
	char stackStart;

	_asm {
		mov index, 0;
		mov stackStart, '$';
		push stackStart;
		mov i, 0;
		mov emptyCounter, 0;
		mov operandEncountered, 0;
		mov prevWasOperator, 0;
	}
	
	// EAX = i al foglalt
	// bl = c ebx foglalt
	_asm {
	mainLoopStart:
		mov eax, i;
		cmp eax, strSize; 
		jge mainLoopEnd; // i>=strSize

		// char c = infix[i];
		movzx ebx, byte ptr[infix + eax];
		mov byte ptr[c], bl;

		// if (c == '-' && (i == 0 || infix[i - 1] == '('))
		cmp bl, '-'; // c == '-'
		jne secondIfCheck;
		cmp eax, 0; // i == 0
		je firstIfIsEquals;
		dec eax;
		movzx ecx, byte ptr[infix + eax];
		inc eax;
		cmp cl, '('; // infix[i - 1] == '('
		jne secondIfCheck;

	firstIfIsEquals:
		mov edi, postfix;
		mov ecx, index;
		mov [edi + ecx], '0'; // postfix[index++] = '0';
		inc index;
		mov edi, postfix;
		mov ecx, index;
		mov[edi + ecx], ' '; // postfix[index++] = ' ';
		inc index;
		mov bl, c;
		push bl; // s.push(c);
		inc emptyCounter;
		jmp ifChecksEnd;

	secondIfCheck:
		// if (isOperand(c))
		push ebx;
		call isOperand; 
		cmp edx, 1;
		pop edx;
		jne thirdIfCheck;

	secondWhile:
		// (i < strSize && isOperand(infix[i])
		cmp eax, strSize; // i < strSize
		jge secondIfInner; 
		mov eax, i;
		movzx edx, byte ptr[infix + eax];
		push dl;
		call isOperand;
		cmp edx, 1; // isOperand(infix[i])
		pop edx;
		jne secondIfInner;
		mov edi, postfix;
		mov ecx, index;
		mov eax, i;
		movzx edx, byte ptr[infix + eax];
		mov[edi + ecx], edx; // postfix[index++] = infix[i++];
		inc index;
		inc i;
		jmp secondWhile;
	secondIfInner:
		mov edi, postfix;
		mov ecx, index;
		mov[edi + ecx], ' '; // postfix[index++] = ' ';
		mov operandEncountered, 1; // operandEncountered = true;
		mov prevWasOperator, 0; // prevWasOperator = false;
		inc index;
		dec i; // --i;
		jmp ifChecksEnd;

	thirdIfCheck:
		cmp bl, '('; //  (c == '(')
		jne forthIfCheck;
		mov bl, c;
		push bl; // s.push(c);
		inc emptyCounter;
		mov prevWasOperator, 0;
		jmp ifChecksEnd;
	forthIfCheck:
		cmp bl, ')'; // (c == ')')
		jne fifthIfCheck;
	forthEmptyErrorIf:
		mov eax, i;
		movzx edx, byte ptr[infix + eax - 1];
		cmp edx, '(';
		jne forthInnerWhile;
		push errorEmpty;
		call printf;
		add esi, 4;
		pop ecx;
		jmp done;
	forthInnerWhile: // while (!s.empty() && s.top() != '(')
		mov edx, dword ptr[esp];
		cmp dl, stackStart;
		je forthInnerIf; // !s.empty()
		mov edx, dword ptr[esp];
		cmp dl, '('; // s.top() != '('
		je forthInnerIf;
		mov edi, postfix;
		mov ecx, index;
		mov edx, dword ptr[esp];
		mov[edi + ecx], edx; // postfix[index++] = s.top();
		inc index;
		mov edi, postfix;
		mov ecx, index;
		mov[edi + ecx], ' '; // postfix[index++] = ' ';
		inc index;
		pop ecx; // s.pop();
		dec emptyCounter;
		jmp forthInnerWhile;
	forthInnerIf:
		mov edx, dword ptr[esp];
		cmp dl, stackStart; // s.empty()
		je forthInnerIfTrue;
		mov edx, dword ptr[esp];
		cmp dl, '('; // s.top() != '('
		jne forthInnerIfTrue;
		pop ecx;
		dec emptyCounter; // s.pop();
		mov prevWasOperator, 0; // prevWasOperator = false;
		jmp ifChecksEnd;
	forthInnerIfTrue: // if (s.empty() || s.top() != '(')
		push errorOpening;
		call printf;
		add esi, 4;
		pop ecx;
		jmp done;

	fifthIfCheck:
		mov bl, c;
		push ebx;
		call isOperator;
		cmp edx, 1; // isOperator(c)
		pop edx;
		jne sixthIfCheck;
	errorOperator:
		cmp prevWasOperator, 1;
		jne errorOperatorExit;
		cmp bl, '-';
		je errorOperatorExit;
		cmp bl, '(';
		je errorOperatorExit;
		push errorConsecutive;
		call printf;
		add esi, 4;
		pop ecx;
		jmp done;
	errorOperatorExit:
	errorOperandEncountered:
		cmp operandEncountered, 0;
		jne errorOperancEncounteredExit;
		cmp bl, '-';
		je errorOperancEncounteredExit;
		cmp bl, '=';
		je errorOperancEncounteredExit;
		push errorWithoutOperand;
		call printf;
		add esi, 4;
		pop ecx;
		jmp done;
	errorOperancEncounteredExit:
		mov operandEncountered, 0;
		mov prevWasOperator, 1;
	errorZero:
		cmp bl, '/';
		jne errorZeroExit;
		cmp[infix + eax + 1], '0';
		jne errorZeroExit;
		push errorZero;
		call printf;
		add esi, 4;
		pop ecx;
		jmp done;
	errorZeroExit:
		
	fifthWhile: // (!s.empty() && precedence(c) <= precedence(s.top()))
		mov edx, dword ptr[esp];
		cmp dl, stackStart; // !s.empty()
		je fifthWhileEnd;
		mov bl, c;
		push ebx;
		call precedence;
		mov ecx, edx; // precedence(c)
		pop edx;
		mov edx, dword ptr[esp];
		push edx;
		call precedence; // precedence(s.top())
		cmp ecx, edx; // precedence(c) <= precedence(s.top())
		pop edx;
		jg fifthWhileEnd;
		mov edx, dword ptr[esp];
		mov edi, postfix;
		mov ecx, index;
		mov[edi + ecx], edx; // postfix[index++] = s.top();
		inc index;
		mov edi, postfix;
		mov ecx, index;
		mov [edi + ecx], ' '; // postfix[index++] = ' ';
		inc index;
		pop ecx; 
		dec emptyCounter; // s.pop();
		jmp fifthWhile;
	fifthWhileEnd:
		mov bl, c;
		push bl; // s.push(c);
		inc emptyCounter;
		jmp ifChecksEnd;
	sixthIfCheck:
		push errorOperator;
		call printf;
		add esi, 4;
		pop ebx;
		jmp done;


	ifChecksEnd:
		// loop END
		inc i;
		jmp mainLoopStart;
	mainLoopEnd:
	lastWhile:
		mov edx, dword ptr[esp];
		cmp dl, stackStart; // (!s.empty())
		je lastWhileEnd;
	lastIfCheck:
		mov edx, dword ptr[esp];
		cmp edx, '(';
		jne lastIfCheckEnd;
		push errorClosing;
		call printf;
		add esi, 4;
		pop ecx;
		jmp done;
	lastIfCheckEnd:
		mov edx, dword ptr[esp];
		mov edi, postfix;
		mov ecx, index;
		mov[edi + ecx], edx;
		inc index;
		mov edi, postfix;
		mov ecx, index;
		mov[edi + ecx], ' ';
		inc index;
		pop ecx;

	lastWhileEnd:
		mov edi, postfix;
		mov ecx, index;
		mov[edi + ecx], '\0';
		inc index;
	}

	const char* postfixMsg = "Postfix kifejezés: %s";
	_asm {
		push postfix;
		push postfixMsg;
		call printf;
		add esp, 4;
	done:
	}

	// Load ESP
	_asm {
		mov esp, saveESP;
	}
	return 0;
}

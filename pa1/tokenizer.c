/*
 * tokenizer.c
 */
#include <stdio.h> // standard
#include <stdlib.h> // to use malloc
#include <string.h> // to use size_t, strcpy, etc.
#include <ctype.h> // to use isdigit, isxdigit, etc.

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */
 
struct TokenizerT_ {
	char* input; // string input pointer
    char* currToken; // current token pointer
    int escape; // indicator if current token ended already. 0 if token is ended, 1 if token is not complete
    size_t strLength; // length of token
    int pos; // position of pointer in string
};

typedef struct TokenizerT_ TokenizerT;

/* 
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts )
{
    TokenizerT* tok = (TokenizerT*) malloc(sizeof(TokenizerT)); // allocating memory
    tok->strLength = strlen(ts);
    if(tok->strLength == 0)
    {
        return NULL;
    }
    tok->escape = 0;
    tok->pos = 0;
    tok->input = (char*)malloc(sizeof(char)*(1+tok->strLength)); // allocating memory
    strcpy(tok->input, ts);
    tok->currToken = (char*)malloc(sizeof(char)*(1+tok->strLength)); // allocating memory
    return tok;
}

/*		
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) { // freeing allocated memory
    free(tk->input);
    free(tk->currToken);
    free(tk);
}

/*
 * isOctal: checks if the next token in TKGetNextToken is an Octal.
 */
 
int isOctal(char* returned, char* stream, int i, int j, TokenizerT * tk)
{
	if (stream[i+1] <= '7' && stream[i+1] >= '0') // OCTAL
	{
		while(stream[i] <= '7' && stream[i] >= '0') // to keep checking digits
		{
			returned[j] = stream[i]; // copy from stream to returned
			i++;
			j++;
		}
		tk->pos = i; // move the pointer to next position for future scan
		
		return 1;
	}
	else if(stream[i] == '0')
	{
		return -1;
	}
	return 0;	
}

 /*
  * isZero: checks if the next token in TKGetNextToken is a Zero.
  */
  
int isZero(char* returned, char* stream, int i, int j, TokenizerT * tk)
{
	if(stream[i+1] != 'x' && stream[i+1] != 'X' && stream[i] == '0')
	{
		returned[j] = stream[i];
		i++;
		j++; 
		tk->pos = i;
		return 1;
	}
	return 0;
}
 
 /*
  * isHex: checks if the next token in TKGetNextToken is a Hex.
  */
 
int isHex(char* returned, char* stream, int i, int j, TokenizerT * tk)
{
	if (stream[i+1] == 'x' || stream[i+1] == 'X') // HEX
	{
		if(isxdigit(stream[i+2])) // to check hex digit 
		{
			while(j<2) // to copy all previous digits and proceed
			{
				returned[j] = stream[i];
				i++;
				j++; 
			}
			while(isxdigit(stream[i]))
			{
			returned[j] = stream[i];
			i++;
			j++;
			}
			tk->pos = i;
			
			return 1;
		}
		else // MALFORMED
		{
			while(j<2) // to copy all previous digits and proceed
			{
				returned[j] = stream[i];
				i++;
				j++;
			}
			tk->pos = i;				
			return 0;
		}
    }
	return 0;
}

 /*
  * isFloat: checks if the next token in TKGetNextToken is a Float (beginning as octal).
  */
  
int isFloat(char* returned, char* stream, int i, int j, TokenizerT * tk)
{
	if(isdigit(stream[i+2]))
	{
		while(j<2) // to copy all previous digits and proceed
		{
			returned[j] = stream[i];
			i++;
			j++;
		}
		while(isdigit(stream[i]))
		{
			returned[j] = stream[i];
			i++;
			j++;
		}
		if(stream[i] == 'E' || stream[i] == 'e')
		{
			if(stream[i+1] == '+' || stream[i+1] == '-')
			{
				if(isdigit(stream[i+2]))
				{
					int x;
					for(x = 0; x<2; x++) // to copy all previous digits and proceed
					{
						returned[j] = stream[i];
						i++;
						j++;
					}
					while(isdigit(stream[i]))
					{
						returned[j] = stream[i];
						i++;
						j++;
					}
					tk->pos = i;
					return 1;
				}
				else
				{
					int x;
					for(x = 0; x<2; x++) // to copy all previous digits and proceed
					{
						returned[j] = stream[i];
						i++;
						j++;
					}
					//MALFORMED
					tk->pos = i;  
					return 0;
				}
			}
			else if (isdigit(stream[i+1]))
			{
				int x;
				for(x = 0; x<2; x++) // to copy all previous digits and proceed
				{
					returned[j] = stream[i];
					i++;
					j++;
				}
				while(isdigit(stream[i])) // keep checking for digits
				{
					returned[j] = stream[i];
					i++;
					j++;
				}
				tk->pos = i;
				return 1;
			}
			else
			{
				returned[j] = stream[i];
				i++;
				j++;
				tk->pos = i;
				return 0;
			}
		}	
		tk->pos = i;
		return 1;
	}
}

 /*
  * isFloat2: checks if the next token in TKGetNextToken is a Float (beginning as decimal).
  */
  
int isFloat2(char* returned, char* stream, int i, int j, TokenizerT * tk)
{
	if(isdigit(stream[i+1]))
	{
		returned[j] = stream[i];
		i++;
		j++;
		while(isdigit(stream[i]))
		{
			returned[j] = stream[i];
			i++;
			j++;
		}
		if(stream[i] == 'E' || stream[i] == 'e')
		{
			if(stream[i+1] == '+' || stream[i+1] == '-')
			{
				if(isdigit(stream[i+2]))
				{
					int x;
					for(x = 0; x<2; x++) // to copy all previous digits and proceed
					{
						returned[j] = stream[i];
						i++;
						j++;
					}
					while(isdigit(stream[i]))
					{
						returned[j] = stream[i];
						i++;
						j++;
					}
					tk->pos = i;
					return 1;
				}
				else
				{
					int x;
					for(x = 0; x<2; x++) // to copy all previous digits and proceed
					{
						returned[j] = stream[i];
						i++;
						j++;
					}
					//MALFORMED
					tk->pos = i;  
					return 0;
				}
			}
			else if (isdigit(stream[i+1]))
			{
				int x;
				for(x = 0; x<2; x++) // to copy all previous digits and proceed
				{
					returned[j] = stream[i];
					i++;
					j++;
				}
				while(isdigit(stream[i])) // keep checking for digits
				{
					returned[j] = stream[i];
					i++;
					j++;
				}
				tk->pos = i;
				return 1;
			}
			else
			{
				returned[j] = stream[i];
				i++;
				j++;
				tk->pos = i;
				return 0;
			}
		}	
		tk->pos = i;
		return 1;
	}
}


int isFloat3(char* returned, char* stream, int i, int j, TokenizerT * tk)
{
	if(isdigit(stream[i+1]))
	{
		returned[j] = stream[i];
		i++;
		j++;
		if(stream[i+1] == '+' || stream[i+1] == '-')
		{
			if(isdigit(stream[i+2]))
			{
				int x;
				for(x = 0; x<2; x++) // to copy all previous digits and proceed
				{
					returned[j] = stream[i];
					i++;
					j++;
				}
				while(isdigit(stream[i]))
				{
					returned[j] = stream[i];
					i++;
					j++;
				}
				tk->pos = i;
				return 1;
			}
			else
			{
				int x;
				for(x = 0; x<2; x++) // to copy all previous digits and proceed
				{
					returned[j] = stream[i];
					i++;
					j++;
				}
				//MALFORMED
				tk->pos = i;  
				return 0;
			}
		}
		else if (isdigit(stream[i+1]))
		{
			int x;
			for(x = 0; x<2; x++) // to copy all previous digits and proceed
			{
				returned[j] = stream[i];
				i++;
				j++;
			}
			while(isdigit(stream[i])) // keep checking for digits
			{
				returned[j] = stream[i];
				i++;
				j++;
			}
			tk->pos = i;
			return 1;
		}
		else
		{
			returned[j] = stream[i];
			i++;
			j++;
			tk->pos = i;
			return 0;
		}
		tk->pos = i;
		return 1;
	}
}

 /*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */
 
char *TKGetNextToken( TokenizerT * tk ) {
    char* returned; // to be returned
    returned = (char*)malloc(sizeof(char)*(1+tk->strLength));
    char* stream = (char*)malloc(sizeof(char)*(1+tk->strLength)); // original input
    strcpy(stream, tk->input);
    strcpy(stream, tk->input);
    int i = tk->pos; // to incerement the pointer position
    int j = 0; // to increment the returned character array
    if (isdigit(stream[i])) 
	{
        // OCTALS and HEX BEGIN:
        if(stream[i] == '0') 
		{
			if(isOctal(returned, stream, i, j, tk) == 1)
			{
				printf("\e[34moctal \e[0m");
				return returned;
			}
            else if(isHex(returned, stream, i, j, tk) == 1)
			{
				printf("\e[34mhex \e[0m");
				return returned;
			}
			// TRANSITION from OCTAL to FLOAT
			else if (stream[i+1]== '.') 
			{
				if(isFloat(returned, stream, i, j, tk) == 1)
				{
					printf("\e[34mfloat \e[0m");
					return returned;
				}
				else
				{
					int x;
					for(x = 0; x<2; x++) // to copy all previous digits and proceed
					{
						returned[j] = stream[i];
						i++;
						j++;
					}
					while(isdigit(stream[i]))
					{
						i++;
					}
					tk->pos = i;
					printf("\e[31mmalformed: \e[0m");
					return returned;
				}
			}
			else if(isZero(returned, stream, i, j, tk) == 1)
			{
				printf("zero ");
				return returned;
			}
			else
			{
				printf("\e[31mmalformed: \e[0m");
				return returned;
			} 
        }
		
        // DECIMALS BEGIN:
		// Determined by a sequence of digits starting with 1-9 and ending with a digit
        else 
		{
            while(isdigit(stream[i]))
            {
                returned[j] = stream[i];
                i++;
                j++;
            }
			// DECIMAL TO FLOAT
            if (stream[i]== '.')
            {
                if(isFloat2(returned, stream, i, j, tk) == 1)
				{
					printf("\e[34mfloat \e[0m");
					return returned;
				}
				else
				{
					int x;
					for(x = 0; x<1; x++) // to copy all previous digits and proceed
					{
						returned[j] = stream[i];
						i++;
						j++;
					}
					while(isdigit(stream[i]))
					{
						i++;
					}
					i++;
					tk->pos = i;
					printf("\e[31mmalformed: \e[0m");
					return returned;
				}
            }
			else if(stream[i] == 'e' || stream[i] == 'E')
			{
				if(isFloat3(returned, stream, i, j, tk) == 1)
				{
					printf("\e[34mfloat \e[0m");
					return returned;
				}
				else
				{
					int x;
					for(x = 0; x<1; x++) // to copy all previous digits and proceed
					{
						returned[j] = stream[i];
						i++;
						j++;
					}
					while(isdigit(stream[i]))
					{
						i++;
					}
					i++;
					tk->pos = i;
					printf("\e[31mmalformed: \e[0m");
					return returned;
				}
			}
            tk->pos = i;
            printf("\e[34mdecimal \e[0m");
            return returned;
        }
    }
	// WHITESPACE REMOVAL:
	else if(stream[i] == ' ' || stream[i] == '\t' || stream[i] == '\v' || stream[i] == '\t' || stream[i] == '\f' || stream[i] == '\r' || stream[i] == '\n')
	{
		tk->escape = 1;
		returned[j] = '\0';
		tk->pos = i+1;
		return returned;
	}
	else 
	{
        tk->escape = 1;
        printf("\e[35mEscape character: [Ox%02x]\e[0m\n", stream[i]);
        returned[j] = '\0';
        i++;
        tk->pos = i;
        return returned;
    } 
    return "0";
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) 
{
    if (argc == 1 || argc > 2)
    {
		printf("\e[31mError: rerun program and re-enter arguments\e[0m\n");
		return 0;
    }
    char* str = argv[1];
    TokenizerT* tok = TKCreate(str); 
	// TO GET TOKENS:
	int k = 0;
	k = tok->strLength;
	while(tok->pos < k)
	{
		tok->currToken = TKGetNextToken(tok);
        if(tok->escape == 1) // if token is not complete (by rules)
        {
			//stop the token at this point and set it to complete, then proceed to get the next token after printing the current state.
            tok->escape = 0;
            continue;
        }
        printf("\"%s\"\n", tok->currToken); // printing the token before getting the next one
	}
    TKDestroy(tok);
    return 0;
}
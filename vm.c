#include <stdio.h>
#include <stdlib.h>

#define MAX_PAS_LENGTH 500

typedef struct instruction
{
    int opcode;
    int l;
    int m;
} instruction;

// Starting variables
int gp, ic, dp, free, pc, sp, bp, program_length, opcode, l, m, halt = 0;
int pas [MAX_PAS_LENGTH + 1] = {0};

int main (int argc, char *argv[])
{
    FILE *fp;

    

    gp = ic;    //Global Pointer – Points to DATA segment
    dp = ic -1;  //Data Pointer – To access variables in Main
    free = ic + 40;  // FREE points to Heap
    bp = ic; // Points to base of DATA or activation records
    pc = 0; // Stack pointer – Points to top of stack  
    sp = MAX_PAS_LENGTH;  
    program_length = 0;
  
    // Initialize all the values to the process address space to zero
    
    
    FILE *fp = NULL;
    fp = fopen(argv[1], "r");
   
    if (fp == NULL)
    {
        return EXIT_FAILURE;
    }
    
    int i = 1;
    while(!feof(fp))
    {
        fscanf(fp, "%d", &pas[i]);
        
        // For checking when the end of a line has been reached
        if ((i % 3 == 0) && (i == 0))
        {
            program_length++;
        }
        
        i++;
    }
    ic = i;

    // Initialize the CPU register values after getting everything into the PAS, updating the values here
    bp = program_length * 3;
    gp = bp;
    dp = ic - 1;
    free = ic + 40;

    i = 0;

    // Moves the counter, i along the PAS so that we can store each of the values as opcode, l, or m
    while (i < (program_length * 3))
    {
        i = pc;

        opcode = pas[i];
        l = pas[i + 1];
        m = pas[i + 2];

        if (halt == 0)
        {
            break;
        }

        switch (opcode)
        {
            case 1:
                if (bp == gp)
                {
                    dp = dp + 1;
                    pas[dp] = m;
                }
                else
                {
                    sp = sp - 1;
                    pas[sp] = m;
                }
                print_execution(i, "LIT", l, m, pc, bp, sp, dp, pas, gp);
                break;
            case 2: 
                switch (m)
                {
                    // RTN
                    case 0:
                        sp = bp + 1;
                        bp = pas[sp - 2];
                        pc = pas[sp - 3];
                        print_execution(i, "RTN", l, m, pc, bp, sp, dp, pas, gp);
                        break;
                    // NEG
                    case 1:
                        if (bp == gp)
                        {
                            pas[dp] = -1 * pas[dp];
                        }
                        else
                        {
                            pas[sp] = -1 * pas[sp];
                        }
                        print_execution(i, "NEG", l, m, pc, bp, sp, dp, pas, gp);
                        break;
                    // ADD
                    case 2:
                        if (bp == gp)
                        {
                            dp = dp - 1;
                            pas[dp] = pas[dp] + pas[dp + 1];
                        }
                        else
                        {
                            sp = sp + 1;
                            pas[sp] = pas[sp] + pas[sp - 1];
                        }
                        print_execution(i, "ADD", l, m, pc, bp, sp, dp, pas, gp);
                        break;
                    // SUB
                    case 3: 
                        if (bp == sp)
                        {
                            dp = dp - 1;
                            pas[dp] = pas[dp] - pas[dp + 1];
                        }
                        else
                        {
                            sp = sp + 1;
                            pas[sp] = pas[sp] + pas[sp - 1];
                        }
                        print_execution(i, "SUB", l, m, pc, bp, sp, dp, pas, gp);
                        break;
                    // MUL
                    case 4: 
                        if (bp == gp)
                        {
                            dp = dp - 1;
                            pas[dp] = pas[dp] + pas[dp + 1];
                        }
                        else
                        {
                            sp = sp + 1;
                            pas[sp] = pas[sp] * pas[sp - 1];
                        }
                        print_execution(i, "MUL", l, m, pc, bp, sp, dp, pas, gp);
                        break;
                    // DIV
                    case 5: 
                        if (bp == gp)
                        {
                            dp = dp - 1;
                            pas[dp] = pas[dp] / pas[dp + 1];
                        }
                        else
                        {
                            sp = sp + 1;
                            pas[sp] = pas[sp] / pas[sp - 1];
                        }
                        print_execution(i, "DIV", l, m, pc, bp, sp, dp, pas, gp);
                        break;
                    // ODD
                    case 6: 
                        if (bp == gp)
                        {
                            pas[dp] = pas[dp] % 2;
                        }
                        else
                        {
                            pas[sp] = pas[sp] % 2;
                        }
                        print_execution(i, "ODD", l, m, pc, bp, sp, dp, pas, gp);
                        break;
                    // MOD
                    case 7: 
                        if (bp == gp)
                        {
                            dp = dp - 1;
                            pas[dp] = pas[dp] % pas[dp + 1];
                        }
                        else
                        {
                            sp = sp + 1;
                            pas[sp] = pas[sp] % pas[sp - 1];
                        }
                        print_execution(i, "MOD", l, m, pc, bp, sp, dp, pas, gp);
                        break;
                    // EQL
                    case 8: 
                        if (bp == gp)
                        {
                            dp = dp - 1;
                            if (pas[dp] == pas[dp + 1])
                            {
                                pas[dp] = 1;
                            }
                            else
                            {
                                pas[dp] = 0;
                            }
                            
                        }
                        else
                        {
                            sp = sp + 1;
                            if (pas[sp] == pas[sp - 1])
                            {
                                pas[sp] = 1;
                            }
                            else
                            {
                                pas[sp] = 0;
                            }
                        }
                        print_execution(i, "EQL", l, m, pc, bp, sp, dp, pas, gp);
                        break;
                    // NEQ
                    case 9: 
                        if (bp == gp)
                        {
                            dp = dp - 1;
                            if (pas[dp] != pas[dp + 1])
                            {
                                pas[dp] = 1;
                            }
                            else
                            {
                                pas[dp] = 0;
                            }
                        }
                        else
                        {
                            sp = sp + 1;
                            if (pas[sp] != pas[sp - 1])
                            {
                                pas[sp] = 1;
                            }
                            else
                            {
                                pas[sp] = 0;
                            }
                        }
                        print_execution(i, "NEQ", l, m, pc, bp, sp, dp, pas, gp);
                        break;    
                    // LSS
                    case 10: 
                        if (bp == gp)
                        {
                            dp = dp - 1;
                            if (pas[dp] < pas[dp + 1])
                            {
                                pas[dp] = 1;
                            }
                            else
                            {
                                pas[dp] = 0;
                            }
                        }
                        else
                        {
                            sp = sp + 1;
                            if (pas[sp] < pas[sp - 1])
                            {
                                pas[sp] = 1;
                            }
                            else
                            {
                                pas[sp] = 0;
                            }
                        }
                        print_execution(i, "LSS", l, m, pc, bp, sp, dp, pas, gp);
                        break; 
                    // LEQ
                    case 11: 
                        if (bp == gp)
                        {
                            dp = dp - 1;
                            if (pas[dp] <= pas[dp + 1])
                            {
                                pas[dp] = 1;
                            }
                            else
                            {
                                pas[dp] = 0;
                            }
                        }
                        else
                        {
                            sp = sp + 1;
                            if (pas[sp] <= pas[sp - 1])
                            {
                                pas[sp] = 1;
                            }
                            else
                            {
                                pas[sp] = 0;
                            }
                        }
                        print_execution(i, "LEQ", l, m, pc, bp, sp, dp, pas, gp);
                        break;
                    case 12:
                    //GTR
                        if(bp == gp)
                        {
                            dp = dp - 1;
                            if (pas[dp] > pas[dp + 1])
                            {
                                pas[dp] = 1;
                            }
                            else
                            {
                                pas[dp] = 0;
                            }
                        }  
                        else 
                        {
                            sp = sp + 1;
                            if (pas[sp] > pas[sp - 1])
                            {
                                pas[sp] = 1;
                            }
                            else
                            {
                                pas[sp] = 0;
                            }

                        }
                        break;
                    case 13:
                    //GEQ
                        if(bp == gp)
                        {
                            dp = dp - 1;
                            if (pas[dp] >= pas[dp + 1])
                            {
                                pas[dp] = 1;
                            }
                            else
                            {
                                pas[dp] = 0;
                            }
                        }  
                        else 
                        {
                            sp = sp + 1;
                            if (pas[sp] >= pas[sp - 1])
                            {
                                pas[sp] = 1;
                            }
                            else
                            {
                                pas[sp] = 0;
                            }

                        }
                        break;   
                }
                case 3:
                //LOD L m
                    if(base(l) == gp)
                    {
                        dp = dp + 1;
                        pas[dp] = pas[base(l) - m];
                    }
                    else
                    {
                        if(base(l) == gp)
                        {
                            sp = sp - 1;
                            pas[sp] = pas[gp + m];          
                        }
                        else
                        {
                            sp = sp - 1;
                            pas[sp] = pas[base(l) - m]; 
                        }
                    }
                    break;

                case 4:
                    //STO L m
                    if(base(l) == gp)
                    {
                        pas[gp + m] = pas[dp];
                        dp = dp - 1;
                        
                    }
                    else
                    {
                        if(base(l) == gp)
                        {
                                
                            sp = sp + 1;
                                
                        }
                        else
                        {
                            pas[base(l) - m] = pas[sp];
                            sp = sp + 1;
                            
                        }
                    }
                    break;

                case 5:
                    //CAL L m
                    pas[sp - 1] = base(l);         // static link (SL)
                    pas[sp - 2] = bp;              // dynamic link (DL)
                    pas[sp - 3] = pc;              // return address (RA)
                    bp = sp - 1;
                    pc = m;

                    break;

                case 6:
                    //INC 0 m
                    if(bp == gp)
                    {
                        gp = gp + m;
                    }
                    else
                    {
                        sp = sp - m;
                    }
                    break;
                    

                case 7:
                    //JmP 0 m
                    pc = m;

                    break;

                case 8:    
                    //JPC 0 m
                    if(bp == gp)
                    {
                        if (pas[dp] == 0)
                        {
                            pc = m;
                        }
                        dp = dp - 1;
                    }
                    else
                    {
                        if (pas[sp] == 0)
                        {
                            pc = m;
                        }
                        sp = sp + 1;
                    }
                    break;
                
                case 9:
                    switch(m)
                    {
                        case 1:
                            //SYS 0 1
                            if(bp == gp)
                            {
                                printf("%d", pas[dp]);
                                dp = dp - 1;
                            }
                            else
                            {
                                printf("%d", pas[sp]);
                                sp = sp + 1;
                            }
                            break;

                        case 2:
                            //SYS 02
                            if(bp == gp)
                            {
                                dp = dp + 1;
                                scanf("%d", pas[dp]);
                            }
                            else
                            {
                                sp = sp - 1;
                                scanf("%d", pas[sp]);
                            }
                            break;

                        case 3:
                            //SYS 03
                            halt = 0;
                            break;
                    }
                
                

            }
    }
    
    


    return 0;
}

void print_execution(int line, char *opname, int l, int m, int PC, int BP, int SP, int DP, int *pas, int GP)
{
    int i; 
    // print out instruction and registers
    printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t", line, opname, l, m, PC, BP, SP, DP);

    // print data section
    for (i = GP; i <= DP; i++)
    {
        printf("%d ", pas[i]);
    }
    printf("\n");
    //print stack
    printf("\tstack : ");
    for (i = MAX_PAS_LENGTH - 1; i >= SP; i--)
    {
        printf("%d ", pas[i]);
    }
    printf("\n");
}


/**********************************************/
/*Find base L levels down */
/*                        */
/**********************************************/
int base(int l)
{
    int arb = bp;      // arb = activation record base
    while (l > 0)     // find base L levels down
    {
        arb = pas[arb];
        l--;
    }
    return arb;
}
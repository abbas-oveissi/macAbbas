typedef unsigned int u32_t;


#define MAX_INTERRUPTS ( 256 )
#define GDT_DESCRIPTORS_NO ( 3 )
#define SYS_CALL_INTERRUPT ( 80 )


typedef struct registers
{
   	u32_t ds;	// Data segment selector
   	u32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;	// Pushed by pusha.
   	u32_t int_no, err_code;	// Interrupt number and error code (if applicable)
   	u32_t eip, cs, eflags, useresp, ss;	// Pushed by the processor automatically.
} registers_t;



typedef void (*isr_t)(registers_t);
       
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

extern void irq_install_handler(int irq, void (*handler)(registers_t *r));
extern void irq_uninstall_handler(int irq);
extern void irq_remap(void);
extern void irq_install();
 

extern unsigned char keyboard_read_scanCode();
extern void terminal_writestring(const char* data);
extern void terminal_initialize();

extern void gdt_init(void);
extern void idt_init(void);
extern void pic_init(void);



extern char* ptrToStr(unsigned int ptr,int base);
extern char * intToStr(unsigned int ptr);
extern void terminal_putchar(char c);

extern void register_isr( unsigned char n, isr_t handler );
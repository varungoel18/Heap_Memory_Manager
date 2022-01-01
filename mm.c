#include<stdio.h>
#include<memory.h>
#include<unistd.h>
#include<sys/mman.h>
static vm_page_for_families_t *first_vm_page_for_families = NULL;
static size_t SYSTEM_PAGE_SIZE = 0;   /* Static global variable declared */

void
mm_init() {
	SYSTEM_PAGE_SIZE = getpagesize();  /* results the virtual memory page size  'getpagesize' is used by the help of unistd.h */

}

/* function to request VM page from kernel */ 
static void *
mm_get_new_vm_page_from_kernel (int units) {
	char *vm_page = mmap(
			0,
			units * SYSTEM_PAGE_SIZE,
			PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_ANON|MAP_PRIVATE,
			0, 0);

	if(vm_page == MAP_FAILED) {
		printf("Error : VM Page allocation Failed \n");
		return NULL;
		
	}
	memset (vm_page, 0,units * SYSTEM_PAGE_SIZE );
	return (void * )vm_page;

}

/* Function to return page to the kernel */

static void 
mm_return_vm_page_to_kernel (void * vm_page , int units) {
	if(munmap(vm_page, units * SYSTEM_PAGE_SIZE) ) {
		printf("Error : Could not munmap VM page to kernel " );
	}
}

void
mm_instantiate_new_page_family(
	char *struct_name,
	uint32_t struct_size) {
	
	vm_page_family_t *vm_page_family_curr = NULL;
	vm_page_for_families_t *new_vm_page_for_families = NULL;

	if(struct_size > SYSTEM_PAGE_SIZE) {
		printf("Error : %s() Structure %s Size exceeds system page size \n" , 
				__FUNCTION__ , struct _name);
		return;
	}
	if(!first_vm_page_for_families) {
		first_vm_page_for_families = (vm_page_for_families_t *)mm_get_new_vm_page_from_kernel(1);
		first_vm_page_for_families->next = NULL;
		strncpy(first_vm_page_for_families-> vm_page_for_family[0].struct_name,
		struct_name, MM_MAX_STRUCT_NAME);
		first_vm_page_for_families->vm_page_family[0].struct_size = struct_size;
		return
	}
	uint32_t count = 0;
	ITERATE_PAGE_FAMILIES_BEGIN (first_vm_page_for_families, vm_page_family_curr) {
		
		if(strncmp(vm_page_family_curr-> struct_name,
			struct_name,MM_MAX_STRUCT_NAME) !=0) {
			count++;
			continue;
		}
		assert(0);
	
	} ITERATE_PAGE_FAMILIES_END(first_vm_page_for_families , vm_page_family_curr);
       if(count == MAX_FAMILIES_PER_VM_PAGE) {
	       new_vm_page_for_families = 
		       (vm_page_for_families_t *)mm_get_new_vm_page_from_kernel(1);
	       new_vm_page_for_families-> next = first_vm_page_for_families;
	       first_vm_page_for_families = new_vm_page_for_families;
	       vm_page_family_curr = &first_vm_page_for_families->vm_page_family[0];
       
       
       
       
    	}
       strncpy(vm_page_family_curr -> struct_name , struct_name ,
		       MM_MAX_STRUCT_NAME);
       vm_page_family_curr -> struct_size = struct_size;
       vm_page_family_curr -> first_page = NULL;

}


int 
main(int argc, char **argv) {
mm_init();
printf("VM page size = %lu\n", SYSTEM_PAGE_SIZE);
void *addr1 = mm_get_new_vm_page_from_kernel(1);
void *addr2 = mm_get_new_vm_page_from_kernel(1);
printf("page 1 = %p,page 2 = %p\n",addr1,addr2);
return 0;
}
  

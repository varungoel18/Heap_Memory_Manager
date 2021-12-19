typedef struct vm_page_family_{
	char struct_name[MM_MAX_STRUCT_NAME];
	uint32_t struct size;
} vm_page_family_t;

typedef struct vm_page_for_families_{
	struct vm_page_for_families_ *next;
	vm_page_family_t vm_page_family[0];
} typedef struct vm_page_for_families_t;


#define MAX FAMILIES PER VM_PAGE	\
	(SYSTEM_PAGE_SIZE - sizeof(vm_page_for_families_t *) /\ sizeof(vm_page_family_t))

#define ITERATE_PAGE_FAMILIES_BEGIN(vm_page_for_families_ptr , curr)	\
{									\
	uint32 t count = 0;						\
	for(curr = (vm_page_family_t *) &vm_page_for_families_ptr->vm_page_family[0];	\
			curr-> struct_size && count < MAX_FAMILIES_PER_VM_PAGE;	\
			curr++ ,count++) {

#define ITERATE_PAGE_FAMILIES_END(vm_page_for_families_ptr , curr) }}

#endif

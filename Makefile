CC ?= gcc
CFLAGS_common ?= -Wall -std=gnu99 -DNDEBUG
CFLAGS_slot = #-DSLOT
CFLAGS_orig = -O0
CFLAGS_opt  = -O0

EXEC = phonebook_orig phonebook_opt
all: $(EXEC)

SRCS_common = main.c

phonebook_orig: $(SRCS_common) phonebook_orig.c phonebook_orig.h
	$(CC) $(CFLAGS_common) $(CFLAGS_orig) \
		-DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c

phonebook_opt: $(SRCS_common) phonebook_opt.c phonebook_opt.h
	$(CC) $(CFLAGS_common) $(CFLAGS_slot) $(CFLAGS_opt) \
		-DIMPL="\"$@.h\"" -o $@ \
		-DOPT="1" \
		$(SRCS_common) $@.c


run: $(EXEC)
	echo 3 | sudo tee /proc/sys/vm/drop_caches
	watch -d -t "./phonebook_opt && echo 3 | sudo tee /proc/sys/vm/drop_caches"

cache-test: $(EXEC)
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles,branch-misses,branch-instructions \
		./phonebook_orig
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles,branch-misses,branch-instructions \
		./phonebook_opt 
orig_perf: $(EXEC)
	perf record -F 10000 \
		-g ./phonebook_orig

opt_perf: $(EXEC)
	perf record -F 10000 \
	-g ./phonebook_opt

ca_orig_perf: $(EXEC)
	perf record -F 10000 \
	-e cache-misses,cache-references,instructions,cycles,branch-misses,branch-instructions \
	-g ./phonebook_orig

ca_opt_perf: $(EXEC)
	perf record -F 10000 \
	-e cache-misses,cache-references,instructions,cycles,branch-misses,branch-instructions \
	-g ./phonebook_opt

output.txt: cache-test calculate
	./calculate

plot: output.txt
	gnuplot scripts/runtime.gp

plot_slot: $(EXEC)
	./phonebook_opt
	gnuplot scripts/slot.gp

calculate: calculate.c
	$(CC) $(CFLAGS_common) $^ -o $@

.PHONY: clean
clean:
	$(RM) $(EXEC) *.o perf.* \
	      	calculate orig.txt opt.txt output.txt runtime.png slot.png slot.txt

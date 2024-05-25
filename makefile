
ALL: calculate

calculate: calculate_c.c
	gcc $^ -o $@ -g


clean:
	rm -rf calculate

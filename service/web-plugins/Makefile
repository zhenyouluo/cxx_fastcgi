PLUGINS=prepare

all: build output

build:
	for plugin in $(PLUGINS) ; do make -C $$plugin ; if [ $$? -ne 0 ]; then exit 1; fi ;done

clean:
	for plugin in $(PLUGINS) ; do make -C $$plugin clean ;done
	rm -rf output

output:
	mkdir -p output/so
	mkdir -p output/conf
	mkdir -p output/dep_so
	for plugin in $(PLUGINS) ; \
			do cp -rf $$plugin/output/so/* output/so ; \
			if [ -d $$plugin/output/conf ] ; \
		   	then \
				cp -rf $$plugin/output/conf/* output/conf ;	\
			fi; \
			if [ -d $$plugin/output/dep_so ] ; \
		   	then \
				cp -rf $$plugin/output/dep_so/* output/dep_so ;	\
			fi; \
	done;


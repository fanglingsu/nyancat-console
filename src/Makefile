include ../config.mk

all: $(TARGET) 

$(TARGET): $(OBJ)
	@echo "$(CC) $@"
	@$(CC) $(FLAGS) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.c $(HEADERS)
	@echo "${CC} $<"
	@$(CC) $(FLAGS) -c -o $@ $<

debug: $(DTARGET)

%.do: %.c $(HEADERS)
	@echo "${CC} $<"
	@$(CC) $(DFLAGS) -c -o $@ $<

$(DTARGET): $(DOBJ) 
	@echo "$(CC) $@"
	@$(CC) $(DFLAGS) $(DOBJ) -o $(DTARGET) $(LIBS)

install: $(TARGET)
	install $(TARGET) $(BINDIR)

uninstall:
	rm -f $(BINDIR)$(TARGET)

clean:
	@echo Cleaning
	@$(RM) $(TARGET) $(DTARGET) $(OBJ) $(DOBJ)

.PHONY: clean all


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

install: $($(TARGET),$(MAN6))
	install $(TARGET) $(BINDIR)
	-[ -e '$(MANDIR)man6' ] || mkdir -p '$(MANDIR)man6' && install -m $(FMOD) $(MAN6) $(MANDIR)man6/

uninstall:
	rm -f $(BINDIR)$(TARGET) $(MANDIR)man6/$(MAN6)

clean:
	@echo Cleaning
	@$(RM) $(TARGET) $(DTARGET) $(OBJ) $(DOBJ)

.PHONY: clean all


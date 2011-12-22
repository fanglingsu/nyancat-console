include ../config.mk

all: $(TARGET) 

$(TARGET): $(OBJ)
	@echo "$(CC) $@"
	@$(CC) $(FLAGS) $(OBJ) -o $(TARGET) $(LIBS)

-include $(OBJ:.o=.d)
-include $(DOBJ:.do=.dd)

%.o: %.c %.h
	@echo "${CC} $<"
	@$(CC) $(FLAGS) -c -o $@ $<

debug: $(DTARGET)

%.do: %.c %.h
	@echo "${CC} $<"
	@$(CC) $(DFLAGS) -c -o $@ $<

$(DTARGET): $(DOBJ) 
	@echo "$(CC) $@"
	@$(CC) $(DFLAGS) $(DOBJ) -o $(DTARGET) $(LIBS)

clean:
	@echo Cleaning
	@$(RM) *.o  
	@$(RM) *.do  
	@$(RM) $(TARGET)
	@$(RM) $(DTARGET)
	@$(RM) *.d

.PHONY: clean all


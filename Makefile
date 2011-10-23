include config.mk

$(TARGET):
	@$(MAKE) -C $(SRCDIR)

clean:
	@$(MAKE) clean -C $(SRCDIR)

debug:
	@$(MAKE) debug -C $(SRCDIR)

.PHONY: clean all

include config.mk

$(TARGET):
	@$(MAKE) -C $(SRCDIR)

clean:
	@$(MAKE) clean -C $(SRCDIR)

debug:
	@$(MAKE) debug -C $(SRCDIR)

install:
	@$(MAKE) install -C $(SRCDIR)

uninstall:
	@$(MAKE) uninstall -C $(SRCDIR)

.PHONY: clean all

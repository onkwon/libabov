include projects/runner.mk

.PHONY: test
test:
	$(Q)$(MAKE) -C $(HALMCU_ROOT)/tests
.PHONY: coverage
coverage:
	$(Q)$(MAKE) -C $(HALMCU_ROOT)/tests $@

.PHONY: docs
docs:
	@LD_LIBRARY_PATH=$$(llvm-config --libdir) $(MAKE) -C $@ clean html

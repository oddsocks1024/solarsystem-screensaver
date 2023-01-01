bin_PROGRAMS = solarsystem
solarsystem_SOURCES = solarsystem.c \
solarsystem_textures.c \
solarsystem_timers.c \
solarsystem.h \
solarsystem_models.h \
solarsystem_random.h \
solarsystem_textures.h \
solarsystem_timers.h

solarsystem_LDFLAGS = @LDFLAGS@ @SDL_LIBS@ @X_PRE_LIBS@ @X_LIBS@ @X_EXTRA_LIBS@ @GLU_LIBS@ @GL_LIBS@
solarsystem_CPPFLAGS = $(all_includes) @SDL_CFLAGS@ @X_CFLAGS@ @GLU_CFLAGS@ @GL_CFLAGS@ 

install-exec-hook:
	if test "@GNOMESAVER@" = "yes" ; then \
	if test -d "$(DESTDIR)@GNOME_LNK_DIR@"; then echo "OK"; else $(MKDIR_P) "$(DESTDIR)@GNOME_LNK_DIR@"; fi; \
	cd "$(DESTDIR)@GNOME_LNK_DIR@"; \
	if test -e solarsystem$(EXEEXT); then rm -f solarsystem$(EXEEXT); else echo "OK"; fi; \
	$(LN_S) $(DESTDIR)$(bindir)/solarsystem$(EXEEXT) solarsystem$(EXEEXT); \
	fi; 

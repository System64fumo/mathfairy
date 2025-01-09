BINS = mathfairy
PKGS = gtkmm-4.0
SRCS +=	$(wildcard src/*.cpp)
OBJS = $(patsubst src/%,$(BUILDDIR)/%,$(SRCS:.cpp=.o))

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
DATADIR ?= $(PREFIX)/share
BUILDDIR = build

CXXFLAGS += -Oz -s -Wall -flto
CXXFLAGS += $(shell pkg-config --cflags $(PKGS))
LDFLAGS += $(shell pkg-config --libs $(PKGS)) -Wl,--gc-sections

$(shell mkdir -p $(BUILDDIR))
JOB_COUNT := $(BINS) $(OBJS)
JOBS_DONE := $(shell ls -l $(JOB_COUNT) 2> /dev/null | wc -l)

define progress
	$(eval JOBS_DONE := $(shell echo $$(($(JOBS_DONE) + 1))))
	@printf "[$(JOBS_DONE)/$(shell echo $(JOB_COUNT) | wc -w)] %s %s\n" $(1) $(2)
endef

$(BINS): $(OBJS)
	$(call progress, Linking $@)
	@$(CXX) -o \
	$(BUILDDIR)/$(BINS) \
	$(OBJS) \
	$(LDFLAGS) \
	$(CXXFLAGS)

$(BUILDDIR)/%.o: src/%.cpp
	$(call progress, Compiling $@)
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

install: $(BINS)
	@echo "Installing..."
	@install -D -t $(DESTDIR)$(BINDIR) $(BUILDDIR)/$(BINS)
	@install -D -t $(DESTDIR)$(DATADIR)/applications data/mathfairy.desktop
	@install -D -t $(DESTDIR)$(DATADIR)/pixmaps data/mathfairy.svg

clean:
	@echo "Cleaning up"
	@rm -r $(BUILDDIR)

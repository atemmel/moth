package compile

import (
	"errors"
	"fmt"
	"os"
	"os/exec"
	"path"
	"path/filepath"
)

type Target int

const (
	Linux   Target = 0
	Windows        = iota
)

type Args struct {
	IncludeDirs []string
	LibDir      string
	OutDir      string
	OutName     string
	Release     bool
	Target      Target
}

func Lib(args Args) error {
	const std = "-std=c++20"
	cwd, _ := os.Getwd()

	outdir := args.OutDir
	if !filepath.IsAbs(outdir) {
		outdir = path.Join(cwd, outdir)
	}

	err := os.Mkdir(args.OutDir, 0644)
	if err != nil {
		if !errors.Is(err, os.ErrExist) {
			return err
		}
	}

	out := path.Join(outdir, args.OutName)
	libdir := "-L" + args.LibDir
	includedirs := args.IncludeDirs

	for i, s := range includedirs {
		includedirs[i] = "-I" + s
	}

	dyn := []string{
		"-fPIC",
		"-shared",
	}

	warn := []string{
		"-Wall",
		"-Wextra",
		"-Wshadow",
		"-Wswitch-enum",
	}

	src := filepath.Join(cwd, "src", "/*.cpp")
	fmt.Println(src)
	srcs, err := filepath.Glob(src)
	if err != nil {
		panic(err)
	}
	fmt.Println(srcs)

	libs := []string{
		libdir,
		"-lmoth",
	}

	arglist := make([]string, 0, 32)

	arglist = append(arglist, dyn...)
	arglist = append(arglist, "-o", out, std)
	arglist = append(arglist, warn...)
	arglist = append(arglist, includedirs...)
	arglist = append(arglist, srcs...)
	arglist = append(arglist, libs...)

	cmd := exec.Command(
		"g++",
		arglist...,
	)

	fmt.Println(arglist)

	cmd.Stderr = os.Stderr
	cmd.Stdout = os.Stdout
	return cmd.Run()
}

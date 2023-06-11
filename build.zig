const std = @import("std");

const Lib = std.Build.LibExeObjStep;
const Target = std.zig.CrossTarget;
const Optimize = std.builtin.OptimizeMode;

const common_flags = &[_][]const u8{
    "-std=c++20",
    "-Wall",
    "-Wextra",
    "-Wshadow",
    "-Wswitch-enum",
};

const lib_flags = &[_][]const u8{
    "-lSDL2",
};

const vendor_path = "vendor/";
const lib_path = vendor_path ++ "lib";
const include_path = vendor_path ++ "include";

fn buildMothLib(b: *std.Build, target: Target, optimize: Optimize) *Lib {
    const lib = b.addStaticLibrary(.{
        .name = "libmoth",
        .target = target,
        .optimize = optimize,
    });
    lib.addCSourceFiles(&.{
        "moth/moth.cpp",
    }, common_flags ++ lib_flags);
    lib.linkLibC();
    lib.linkLibCpp();
    lib.addLibraryPath(lib_path);
    lib.addIncludePath(include_path);
    return lib;
}

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const libmoth = buildMothLib(b, target, optimize);
    const exe = b.addExecutable(.{
        .name = "moth",
        .target = target,
        .optimize = optimize,
    });
    exe.addIncludePath("loader");
    exe.addCSourceFiles(&.{
        "loader/main.cpp",
    }, common_flags);
    exe.linkLibC();
    exe.linkLibCpp();
    exe.linkLibrary(libmoth);
    b.installArtifact(exe);
}

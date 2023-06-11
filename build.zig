const std = @import("std");

const common_flags = &[_][]const u8{
    "-std=c++20",
    "-Wall",
    "-Wextra",
    "-Wshadow",
    "-Wswitch-enum",
};

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "moth",
        .root_source_file = .{ .path = "loader/main.cpp" },
        .target = target,
        .optimize = optimize,
    });

    exe.addIncludePath("loader");
    exe.addCSourceFile("loader/main.cpp", common_flags);

    exe.linkLibCpp();
    b.installArtifact(exe);
}

fn findCpp() void {
    var root = std.fs.cwd().openIterableDir(".", .{});
    _ = root;
}

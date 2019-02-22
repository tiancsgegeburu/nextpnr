/*
 *  nextpnr -- Next Generation Place and Route
 *
 *  Copyright (C) 2018  Miodrag Milanovic <miodrag@symbioticeda.com>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "project.h"
#include <boost/filesystem/convenience.hpp>
#include <fstream>
#include "log.h"
#include "pcf.h"

NEXTPNR_NAMESPACE_BEGIN

void ProjectHandler::saveArch(Context *ctx, pt::ptree &root, std::string path)
{
    root.put("project.arch.package", ctx->archArgs().package);
    if (ctx->settings.find(ctx->id("input/pcf")) != ctx->settings.end()) {
        std::string fn = ctx->settings[ctx->id("input/pcf")];
        root.put("project.input.pcf", make_relative(fn, path).string());
    }
}

std::unique_ptr<Context> ProjectHandler::createContext(pt::ptree &root)
{
    ArchArgs chipArgs;
    std::string arch_type = root.get<std::string>("project.arch.type");
    if (arch_type == "lp384") {
        chipArgs.type = ArchArgs::LP384;
    }
    if (arch_type == "lp1k") {
        chipArgs.type = ArchArgs::LP1K;
    }
    if (arch_type == "lp8k") {
        chipArgs.type = ArchArgs::LP8K;
    }
    if (arch_type == "hx1k") {
        chipArgs.type = ArchArgs::HX1K;
    }
    if (arch_type == "hx8k") {
        chipArgs.type = ArchArgs::HX8K;
    }
    if (arch_type == "up5k") {
        chipArgs.type = ArchArgs::UP5K;
    }
    if (arch_type == "u4k") {
        chipArgs.type = ArchArgs::U4K;
    }
    chipArgs.package = root.get<std::string>("project.arch.package");

    return std::unique_ptr<Context>(new Context(chipArgs));
}

void ProjectHandler::loadArch(Context *ctx, pt::ptree &root, std::string path)
{
    auto input = root.get_child("project").get_child("input");
    boost::filesystem::path pcf = boost::filesystem::path(path) / input.get<std::string>("pcf");
    std::ifstream f(pcf.string());
    if (!apply_pcf(ctx, input.get<std::string>("pcf"), f))
        log_error("Loading PCF failed.\n");
}

NEXTPNR_NAMESPACE_END

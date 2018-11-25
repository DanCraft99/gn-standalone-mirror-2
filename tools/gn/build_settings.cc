// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "tools/gn/build_settings.h"

#include <utility>

#include "base/files/file_util.h"
#include "tools/gn/filesystem_utils.h"
#include "util/exe_path.h"

BuildSettings::BuildSettings() = default;

BuildSettings::BuildSettings(const BuildSettings& other)
    : dotfile_name_(other.dotfile_name_),
      root_path_(other.root_path_),
      root_path_utf8_(other.root_path_utf8_),
      secondary_source_path_(other.secondary_source_path_),
      python_path_(other.python_path_),
      build_config_file_(other.build_config_file_),
      build_config_root_path_(other.build_config_root_path_),
      build_config_root_path_utf8_(other.build_config_root_path_utf8_),
      arg_file_template_path_(other.arg_file_template_path_),
      build_dir_(other.build_dir_),
      build_args_(other.build_args_) {}

BuildSettings::~BuildSettings() = default;

void BuildSettings::SetRootTargetLabel(const Label& r) {
  root_target_label_ = r;
}

void BuildSettings::SetRootPath(const base::FilePath& r) {
  DCHECK(r.value()[r.value().size() - 1] != base::FilePath::kSeparators[0]);
  root_path_ = r.NormalizePathSeparatorsTo('/');
  root_path_utf8_ = FilePathToUTF8(root_path_);
}

void BuildSettings::SetBuildConfigRootPath(const base::FilePath& r) {
  DCHECK(r.value()[r.value().size() - 1] != base::FilePath::kSeparators[0]);
  if (r == UTF8ToFilePath("GN_INSTALL_PATH"))
  {
    build_config_root_path_ = GetExePath().DirName().NormalizePathSeparatorsTo('/');
    build_config_root_path_utf8_ = FilePathToUTF8(build_config_root_path_);
  }
  else
  {
    build_config_root_path_ = r.NormalizePathSeparatorsTo('/');
    build_config_root_path_utf8_ = FilePathToUTF8(build_config_root_path_);
  }
}

void BuildSettings::SetSecondarySourcePath(const SourceDir& d) {
  secondary_source_path_ = GetFullPath(d).NormalizePathSeparatorsTo('/');
}

void BuildSettings::SetBuildDir(const SourceDir& d) {
  build_dir_ = d;
}

base::FilePath BuildSettings::GetFullPath(const SourceFile& file) const {
  return file.Resolve(root_path_).NormalizePathSeparatorsTo('/');
}

base::FilePath BuildSettings::GetFullPath(const SourceDir& dir) const {
  return dir.Resolve(root_path_).NormalizePathSeparatorsTo('/');
}

base::FilePath BuildSettings::GetFullPath(const std::string& path,
                                          bool as_file) const {
  return ResolvePath(path, as_file, root_path_).NormalizePathSeparatorsTo('/');
}

base::FilePath BuildSettings::GetFullPathSecondary(
    const SourceFile& file) const {
  return file.Resolve(secondary_source_path_).NormalizePathSeparatorsTo('/');
}

base::FilePath BuildSettings::GetFullPathSecondary(const SourceDir& dir) const {
  return dir.Resolve(secondary_source_path_).NormalizePathSeparatorsTo('/');
}

base::FilePath BuildSettings::GetFullPathSecondary(const std::string& path,
                                                   bool as_file) const {
  return ResolvePath(path, as_file, secondary_source_path_)
      .NormalizePathSeparatorsTo('/');
}

base::FilePath BuildSettings::GetFullPathBuildConfigRoot(const SourceFile& file) const
{
  return file.Resolve(build_config_root_path_).NormalizePathSeparatorsTo('/');
}

base::FilePath BuildSettings::BuildSettings::GetFullPathBuildConfigRoot(const SourceDir& dir) const
{
  return dir.Resolve(build_config_root_path_).NormalizePathSeparatorsTo('/');
}

base::FilePath BuildSettings::GetFullPathBuildConfigRoot(const std::string& path, bool as_file) const
{
  return ResolvePath(path, as_file, build_config_root_path_).NormalizePathSeparatorsTo('/');
}

void BuildSettings::ItemDefined(std::unique_ptr<Item> item) const {
  DCHECK(item);
  if (!item_defined_callback_.is_null())
    item_defined_callback_.Run(std::move(item));
}

// SPDX-License-Identifier: AGPL-3.0-only
//! Wutong Publisher engine.
//! Build planning: map native sources to locally installed toolchains (no network, OFFLINE-001).

/// Supported source formats.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Format {
    /// Markdown
    Markdown,
    /// Typst
    Typst,
    /// LaTeX
    Latex,
    /// AsciiDoc
    AsciiDoc,
    /// reStructuredText
    Rst,
    /// Org mode
    Org,
}

/// Detect by extension.
pub fn detect(path: &str) -> Option<Format> {
    let ext = path.rsplit_once('.')?.1.to_ascii_lowercase();
    Some(match ext.as_str() {
        "md" | "markdown" => Format::Markdown,
        "typ" => Format::Typst,
        "tex" => Format::Latex,
        "adoc" | "asciidoc" => Format::AsciiDoc,
        "rst" => Format::Rst,
        "org" => Format::Org,
        _ => return None,
    })
}

/// External tool + arguments for a PDF build (argv form, never a shell string).
pub fn pdf_command(f: Format, input: &str, output: &str) -> Vec<String> {
    let v = |a: &[&str]| a.iter().map(|s| s.to_string()).collect();
    match f {
        Format::Typst => v(&["typst", "compile", input, output]),
        Format::Latex => v(&["latexmk", "-pdf", "-interaction=nonstopmode", input]),
        Format::AsciiDoc => v(&["asciidoctor-pdf", "-o", output, input]),
        Format::Markdown | Format::Rst | Format::Org => v(&["pandoc", input, "-o", output]),
    }
}

/// Is the tool available on PATH?
pub fn tool_available(tool: &str) -> bool {
    std::env::var_os("PATH").is_some_and(|p| {
        std::env::split_paths(&p)
            .any(|d| d.join(tool).is_file() || d.join(format!("{tool}.exe")).is_file())
    })
}

/// One-line description used by the GUI.
pub fn describe_path(path: &str) -> String {
    match detect(path) {
        Some(f) => {
            let cmd = pdf_command(f, path, "out.pdf");
            let ok = tool_available(&cmd[0]);
            format!(
                "{f:?} → PDF via `{}` ({})",
                cmd.join(" "),
                if ok { "available" } else { "not installed" }
            )
        }
        None => "unsupported source format".into(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn detect_and_plan() {
        assert_eq!(detect("book/main.typ"), Some(Format::Typst));
        assert_eq!(detect("a.TEX"), Some(Format::Latex));
        assert_eq!(detect("noext"), None);
        assert_eq!(
            pdf_command(Format::Markdown, "a.md", "a.pdf"),
            ["pandoc", "a.md", "-o", "a.pdf"]
        );
    }
}

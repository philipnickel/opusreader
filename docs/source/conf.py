# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

from pathlib import Path

from exhale import utils as exhale_utils
# -- Path setup --------------------------------------------------------------
ROOT_DIR = Path(__file__).resolve().parents[2]
SOURCE_DIR = Path(__file__).resolve().parent

# -- Project information -----------------------------------------------------

project = 'OpusReader'
copyright = '2025, OpusReader Developers'
author = 'OpusReader Developers'
release = '2.0.0'

# -- General configuration ---------------------------------------------------

extensions = [
    'breathe',
    'exhale',
    'sphinx_rtd_theme',
]

templates_path = ['_templates']
exclude_patterns = []

language = 'en'

# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

# -- Breathe / Exhale configuration -----------------------------------------

DOXYGEN_XML_DIR = ROOT_DIR / 'docs_generated' / 'xml'

breathe_source_exists = DOXYGEN_XML_DIR.exists()
if not breathe_source_exists:
    raise RuntimeError(
        f"Doxygen XML output not found at {DOXYGEN_XML_DIR}. "
        "Run `make docs` from the repository root to generate it."
    )

breathe_projects = {
    'OpusReader': str(DOXYGEN_XML_DIR),
}
breathe_default_project = 'OpusReader'

exhale_args = {
    'containmentFolder': str(SOURCE_DIR / 'api'),
    'rootFileName': 'api_reference.rst',
    'rootFileTitle': 'API Reference',
    'doxygenStripFromPath': str((ROOT_DIR / 'pdf_viewer').resolve()),
    'createTreeView': True,
    'customSpecificationsMapping': exhale_utils.makeCustomSpecificationsMapping(
        lambda kind: [
            ':members:',
            ':protected-members:',
        ] if kind in {'class', 'struct'} else []
    ),
}

primary_domain = 'cpp'
highlight_language = 'cpp'

suppress_warnings = [
    'cpp.duplicate_declaration',
    'cpp.parse',
]

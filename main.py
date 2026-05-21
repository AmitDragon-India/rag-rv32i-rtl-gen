"""Entry point for RAG-RV32I RTL Generation pipeline."""

import sys
import logging
import argparse

from app.config import settings


def setup_logging():
    """Configure logging for the application."""
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s | %(levelname)-8s | %(name)s | %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S",
    )


def run_api():
    """Run FastAPI server with uvicorn."""
    import uvicorn
    from app.config import settings

    uvicorn.run(
        "app.api:app",
        host=settings.server.host,
        port=settings.server.port,
        reload=False,
        log_level="info",
    )


def run_ui():
    """Run Gradio UI."""
    from app.ui import launch_ui

    launch_ui()


def run_eval():
    """Run retrieval evaluation — compare dense vs hybrid."""
    from app.evaluate import (
        RetrievalEvaluator,
        print_comparison,
        save_report,
    )

    evaluator = RetrievalEvaluator()
    comparison = evaluator.compare_modes()
    print_comparison(comparison)
    save_report(comparison, "outputs/eval_report.json")


def main():
    parser = argparse.ArgumentParser(description="RAG-RV32I RTL Generation Pipeline")
    parser.add_argument(
        "mode",
        choices=["api", "ui", "eval"],
        help="Run mode: 'api' for FastAPI server, 'ui' for Gradio interface, 'eval' for evaluation",
    )
    args = parser.parse_args()

    setup_logging()

    if args.mode == "api":
        run_api()
    elif args.mode == "ui":
        run_ui()
    elif args.mode == "eval":
        run_eval()


if __name__ == "__main__":
    main()

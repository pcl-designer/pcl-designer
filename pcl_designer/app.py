"""Flask application: serves the form and the /api/optimize endpoint."""

from __future__ import annotations

from flask import Flask, jsonify, render_template, request

from . import __version__
from .runner import run_optimization
from .validation import ValidationError, validate_payload


def create_app() -> Flask:
    app = Flask(__name__)

    @app.get("/")
    def index():
        return render_template("index.html", version=__version__)

    @app.get("/api/health")
    def health():
        return jsonify({"ok": True, "version": __version__})

    @app.post("/api/optimize")
    def optimize():
        try:
            params = validate_payload(request.get_json(silent=True))
        except ValidationError as e:
            return jsonify({"ok": False, "error": str(e)}), 400

        try:
            result = run_optimization(params, timeout_sec=params["timeout_sec"])
        except RuntimeError as e:
            return jsonify({"ok": False, "error": str(e)}), 500

        return jsonify({
            "ok": True,
            "result": result,
            "echo": {
                "m": params["m"],
                "K": params["K"],
                "n_sizes": params["n_sizes"],
                "num_factors": params["num_factors"],
                "num_wp_factors": params["num_wp_factors"],
                "num_sp_factors": params["num_sp_factors"],
                "term_labels": params["term_labels"],
                "main_effect_term_indices": params["main_effect_term_indices"],
                "p_dim": params["p_dim"],
                "eval_method": params["eval_method"],
                "sigma2_fixed": params["sigma2_fixed"],
                "lambda_fixed": params["lambda_fixed"],
                "seed": params["seed"],
                "num_starts": params["num_starts"],
            },
        })

    return app


app = create_app()

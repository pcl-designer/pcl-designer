"""Smoke tests for app.py routes via Flask test client."""

from pcl_designer.app import create_app


def test_health_endpoint():
    client = create_app().test_client()
    r = client.get("/api/health")
    assert r.status_code == 200
    data = r.get_json()
    assert data["ok"] is True
    assert "version" in data


def test_index_renders():
    client = create_app().test_client()
    r = client.get("/")
    assert r.status_code == 200
    assert b"PCL Designer" in r.data


def test_optimize_rejects_empty_body():
    client = create_app().test_client()
    r = client.post("/api/optimize", json={})
    assert r.status_code == 400
    assert r.get_json()["ok"] is False


def test_optimize_rejects_missing_fields():
    client = create_app().test_client()
    r = client.post("/api/optimize", json={"m": 10})
    assert r.status_code == 400
    body = r.get_json()
    assert body["ok"] is False
    assert "error" in body

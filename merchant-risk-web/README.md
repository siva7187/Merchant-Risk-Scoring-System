# Merchant Risk Scoring System — Web App

Flask app implementing PS-05: input form + JSON API, weighted risk scoring engine,
dashboard sorted by risk with drill-down, and threshold-based alerts.

## Run it

```bash
pip install flask
python3 app.py
```

Then open http://127.0.0.1:5050

## Structure

- `risk_engine.py` — pure scoring logic (validation, ratios, weighted score, classification, alerts)
- `app.py` — Flask routes: JSON API + server-rendered pages
- `templates/` — dashboard, add-merchant form, drill-down detail page
- `static/style.css` — design system

## API

| Method | Route                   | Description                          |
|--------|--------------------------|---------------------------------------|
| POST   | `/api/merchants`         | Submit a merchant profile, get scored result |
| GET    | `/api/merchants`         | List all merchants, sorted by risk score desc |
| GET    | `/api/merchants/<id>`    | Full breakdown for one merchant       |
| GET    | `/api/alerts`            | Merchants with active threshold alerts |
| GET    | `/api/business-types`    | Valid business type values            |

Example:
```bash
curl -X POST http://127.0.0.1:5050/api/merchants \
  -H "Content-Type: application/json" \
  -d '{"name":"ABC Electronics","businessType":"Electronics","businessAge":4,
       "transactions":1000,"chargebacks":60,"refunds":150,"fraudCases":2}'
```

## Notes

- Storage is in-memory (dict), resets on restart — swap in SQLite/Postgres for persistence.
- Scoring weights/thresholds match the original spec: Chargeback 30%, Refund 20%,
  Fraud 20%, Business Type 20%, Age 5%, Volume 5%.
